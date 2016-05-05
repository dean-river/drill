#ifndef DRILL_EXT_REDIS_REDIS_CLIENT_H_
#define DRILL_EXT_REDIS_REDIS_CLIENT_H_
#include <drill/event/event_loop.h>
#include <drill/event/channel.h>
#include <drill/net/addr_inet.h>
#include <drill/common/time.h>

#include <memory>
#include <functional>
#include <hiredis/hiredis.h>
#pragma GCC diagnostic ignored "-Wconversion"

struct redisAsyncContext;

using namespace drill::event;
using namespace drill::net;
using namespace drill::common;

namespace drill {
namespace redis {

class RedisClient {
 public:
  typedef std::function<void(RedisClient*, int)> ConnectCallback;
  typedef std::function<void(RedisClient*, int)> DisconnectCallback;
  typedef std::function<void(RedisClient*, redisReply*)> CommandCallback;

  RedisClient(EventLoop* loop, const AddrInet& serverAddr);
  ~RedisClient();

  const AddrInet& serverAddress() const { return _serverAddr; }

  redisAsyncContext* context() { return _context; }
  bool connected() const;
  const char* errstr() const;

  void setConnectCallback(const ConnectCallback& cb) { _connectCb = cb; }
  void setDisconnectCallback(const DisconnectCallback& cb) { _disconnectCb = cb; }

  void connect();
  void disconnect();  // FIXME: implement this with redisAsyncDisconnect

  int command(const CommandCallback& cb, std::string cmd, ...);

  int ping();

 private:
  void handleRead(Time receiveTime);
  void handleWrite();

  int fd() const;
  void logConnection(bool up) const;
  void setChannel();
  void removeChannel();

  void connectCallback(int status);
  void disconnectCallback(int status);
  void commandCallback(redisReply* reply, CommandCallback* privdata);

  static RedisClient* getRedisClient(const redisAsyncContext* ac);

  static void connectCallback(const redisAsyncContext* ac, int status);
  static void disconnectCallback(const redisAsyncContext* ac, int status);
  // command callback
  static void commandCallback(redisAsyncContext* ac, void*, void*);

  static void addRead(void* privdata);
  static void delRead(void* privdata);
  static void addWrite(void* privdata);
  static void delWrite(void* privdata);
  static void cleanup(void* privdata);

  void pingCallback(RedisClient* me, redisReply* reply);

 private:
  EventLoop* _loop;
  AddrInet   _serverAddr;
  redisAsyncContext* _context;
  std::shared_ptr<Channel> _channel;
  ConnectCallback _connectCb;
  DisconnectCallback _disconnectCb;
};

}
}
#endif  // MUDUO_EXAMPLES_HIREDIS_HIREDIS_H
