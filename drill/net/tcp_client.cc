#include <drill/net/tcp_client.h>
#include <drill/common/logger.h>
#include <drill/net/connector.h>
#include <drill/net/socket.h>
#include <functional>

#include <cstdio>  // snprintf


using namespace drill::common;
using namespace drill::event;


namespace drill{
namespace util {
void removeConnection(EventLoop* loop, const drill::net::TcpConnectionPtr& conn)
{
  loop->queueInLoop(std::bind(&drill::net::TcpConnection::connectDestroyed, conn));
}

void removeConnector(const drill::net::ConnectorPtr& connector)
{
	(void)connector;
  //connector->
}

}

namespace net {
	
TcpClient::TcpClient(EventLoop* loop,
                      AddrInet& serverAddr,
                     const std::string &name)
  : _loop(loop),
  	_serveraddr(serverAddr),
    _connector(new Connector(loop, serverAddr)),
    _name(name),
    _connectionCallback(defaultConnectionCallback),
    _messageCallback(defaultMessageCallback),
    _retry(false),
    _connect(false),
    _nextConnId(1)
{
	LOG_DEBUG<<"TcpClient::TcpClient ["<<_name<<"]";
  _connector->setNewConnectionCallback(
      std::bind(&TcpClient::newConnection, this, std::placeholders::_1));
}

TcpClient::~TcpClient()
{
	LOG_DEBUG<<"TcpClient::~TcpClient ["<<_name<<"]";

  TcpConnectionPtr conn;
  bool unique = false;
  {
    ScopedMutex lock(&_mutex);
    unique = _connection.unique();
    conn = _connection;
  }
  if (conn) {
    assert(_loop == conn->getLoop());
    // FIXME: not 100% safe, if we are in different thread
    CloseCallback cb = std::bind(&drill::util::removeConnection, _loop, std::placeholders::_1);
    _loop->runInLoop(
        std::bind(&TcpConnection::setCloseCallback, conn, cb));
    	if (unique) {
      		 conn->forceClose();
    	}
  } else {
    	_connector->stop();
 	   _loop->runAfter(1, std::bind(&drill::util::removeConnector, _connector));
  }
}

void TcpClient::connect()
{
	LOG_DEBUG<<"TcpClient::connect ["<<_name<<"]";
  _connect = true;
  _connector->start();
}

void TcpClient::disconnect()
{
	LOG_DEBUG<<"TcpClient::disconnect ["<<_name<<"]";

  _connect = false;

  {
    ScopedMutex lock(&_mutex);
    if (_connection)
    {
      _connection->shutdown();
    }
  }
}

void TcpClient::stop()
{
	LOG_DEBUG<<"TcpClient::stop ["<<_name<<"]";
  _connect = false;
  _connector->stop();
}

void TcpClient::newConnection(int sockfd)
{
	LOG_DEBUG<<"TcpClient::newConnection ["<<_name<<"]";
  _loop->assertInLoopThread();
  char buf[32];
  snprintf(buf, sizeof buf, ":%s#%d", _serveraddr.toString().c_str(), _nextConnId);
  ++_nextConnId;
  string connName = _name + buf;

  AddrInet localAddr;
  localAddr.upDateLocal(sockfd);
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(_loop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          _serveraddr));

  conn->setConnectionCallback(_connectionCallback);
  conn->setMessageCallback(_messageCallback);
  conn->setWriteCompleteCallback(_writeCompleteCallback);
  conn->setCloseCallback(
      std::bind(&TcpClient::removeConnection, this,std::placeholders::_1)); // FIXME: unsafe
  {
    ScopedMutex lock(&_mutex);
    _connection = conn;
  }
  conn->connectEstablished();
}

void TcpClient::removeConnection(const TcpConnectionPtr& conn)
{
	LOG_DEBUG<<"TcpClient::removeConnection ["<<_name<<"]";
  _loop->assertInLoopThread();
  assert(_loop == conn->getLoop());

  {
    ScopedMutex lock(&_mutex);
    assert(_connection == conn);
    _connection.reset();
  }

  _loop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
  if (_retry && _connect)
  {
    _connector->restart();
  }
}
}
}
