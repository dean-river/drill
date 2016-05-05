#include "redis_client.h"
#include <drill/common/logger.h>
#include <drill/net/socket.h>
#include <hiredis/async.h>
#pragma GCC diagnostic ignored "-Wconversion"


using namespace drill::event;
using namespace drill::net;
using namespace drill::common;
using namespace drill::redis;

static void dummy(const std::shared_ptr<Channel>&)
{
}

RedisClient::RedisClient(EventLoop* loop, const AddrInet& serverAddr)
  : _loop(loop),
    _serverAddr(serverAddr),
    _context(NULL)
{
}

RedisClient::~RedisClient()
{
  LOG_DEBUG << this;
  assert(!_channel || _channel->isNoneEvent());
  ::redisAsyncFree(_context);
}

bool RedisClient::connected() const
{
  return _channel && _context && (_context->c.flags & REDIS_CONNECTED);
}

const char* RedisClient::errstr() const
{
  assert(_context != NULL);
  return _context->errstr;
}

void RedisClient::connect()
{
  assert(!_context);

  _context = ::redisAsyncConnect(_serverAddr.toString().c_str(), _serverAddr.port());

  _context->ev.addRead = addRead;
  _context->ev.delRead = delRead;
  _context->ev.addWrite = addWrite;
  _context->ev.delWrite = delWrite;
  _context->ev.cleanup = cleanup;
  _context->ev.data = this;

  setChannel();

  assert(_context->onConnect == NULL);
  assert(_context->onDisconnect == NULL);
  ::redisAsyncSetConnectCallback(_context, connectCallback);
  ::redisAsyncSetDisconnectCallback(_context, disconnectCallback);
}

void RedisClient::disconnect()
{
  if (connected())
  {
    LOG_DEBUG << this;
    ::redisAsyncDisconnect(_context);
  }
}

int RedisClient::fd() const
{
  assert(_context);
  return _context->c.fd;
}

void RedisClient::setChannel()
{
  LOG_DEBUG << this;
  assert(!_channel);
  _channel.reset(new Channel(_loop, fd()));
  _channel->setReadCallback(std::bind(&RedisClient::handleRead, this, std::placeholders::_1));
  _channel->setWriteCallback(std::bind(&RedisClient::handleWrite, this));
}

void RedisClient::removeChannel()
{
  LOG_DEBUG << this;
  _channel->disableAll();
  _channel->remove();
  _loop->queueInLoop(std::bind(dummy, _channel));
  _channel.reset();
}

void RedisClient::handleRead(Time receiveTime)
{
  LOG_TRACE << "receiveTime = " << receiveTime.toDateTime();
  ::redisAsyncHandleRead(_context);
}

void RedisClient::handleWrite()
{
  if (!(_context->c.flags & REDIS_CONNECTED))
  {
    removeChannel();
  }
  ::redisAsyncHandleWrite(_context);
}

/* static */ RedisClient* RedisClient::getRedisClient(const redisAsyncContext* ac)
{
  RedisClient* hiredis = static_cast<RedisClient*>(ac->ev.data);
  assert(hiredis->_context == ac);
  return hiredis;
}

void RedisClient::logConnection(bool up) const
{
  AddrInet localAddr;
  localAddr.upDateLocal(fd());

  LOG_INFO << localAddr.toString(true) << " -> "
           << _serverAddr.toString(true) << " is "
           << (up ? "UP" : "DOWN");
}

/* static */ void RedisClient::connectCallback(const redisAsyncContext* ac, int status)
{
  getRedisClient(ac)->connectCallback(status);
}

void RedisClient::connectCallback(int status)
{
  if (status != REDIS_OK)
  {
    LOG_ERROR << _context->errstr << " failed to connect to " << _serverAddr.toString(true);
  }
  else
  {
    logConnection(true);
    setChannel();
  }

  if (_connectCb)
  {
    _connectCb(this, status);
  }
}

/* static */ void RedisClient::disconnectCallback(const redisAsyncContext* ac, int status)
{
  LOG_TRACE;
  getRedisClient(ac)->disconnectCallback(status);
}

void RedisClient::disconnectCallback(int status)
{
  logConnection(false);
  removeChannel();

  if (_disconnectCb)
  {
    	_disconnectCb(this, status);
  }
}

void RedisClient::addRead(void* privdata)
{
  LOG_TRACE;
  RedisClient* hiredis = static_cast<RedisClient*>(privdata);
  hiredis->_channel->enableReading();
}

void RedisClient::delRead(void* privdata)
{
  LOG_TRACE;
  RedisClient* hiredis = static_cast<RedisClient*>(privdata);
  hiredis->_channel->disableReading();
}

void RedisClient::addWrite(void* privdata)
{
  LOG_TRACE;
  RedisClient* hiredis = static_cast<RedisClient*>(privdata);
  hiredis->_channel->enableWriting();
}

void RedisClient::delWrite(void* privdata)
{
  LOG_TRACE;
  RedisClient* hiredis = static_cast<RedisClient*>(privdata);
  hiredis->_channel->disableWriting();
}

void RedisClient::cleanup(void* privdata)
{
  RedisClient* hiredis = static_cast<RedisClient*>(privdata);
  LOG_DEBUG << hiredis;
}

int RedisClient::command(const CommandCallback& cb, std::string cmd, ...)
{
  if (!connected()) return REDIS_ERR;

  LOG_TRACE;
  CommandCallback* p = new CommandCallback(cb);
  va_list args;
  va_start(args, cmd);
  int ret = ::redisvAsyncCommand(_context, commandCallback, p, cmd.c_str(), args);
  va_end(args);
  return ret;
}

/* static */ void RedisClient::commandCallback(redisAsyncContext* ac, void* r, void* privdata)
{
  redisReply* reply = static_cast<redisReply*>(r);
  CommandCallback* cb = static_cast<CommandCallback*>(privdata);
  getRedisClient(ac)->commandCallback(reply, cb);
}

void RedisClient::commandCallback(redisReply* reply, CommandCallback* cb)
{
  (*cb)(this, reply);
  delete cb;
}

int RedisClient::ping()
{
	CommandCallback cb= std::bind(&RedisClient::pingCallback, this, std::placeholders::_1, std::placeholders::_2);
  	return command(cb, std::string("PING"));
}

void RedisClient::pingCallback(RedisClient* me, redisReply* reply)
{
  assert(this == me);
  LOG_DEBUG << reply->str;
}
