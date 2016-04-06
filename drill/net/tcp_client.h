#ifndef DRILL_NET_TCPCLIENT_H_
#define DRILL_NET_TCPCLIENT_H_
#include <drill/common/noncopyable.h>
#include <drill/common/mutex.h>
#include <drill/common/scoped_lock.h>

#include <drill/net/tcp_connection.h>
#include <drill/net/addr_inet.h>
#include <drill/event/event_loop.h>
#include <memory>
#include <string>

using namespace drill::common;
using namespace drill::event;

namespace drill {
namespace net{

class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable
{
 public:
  TcpClient(EventLoop* loop,
             AddrInet& serverAddr,
            const std::string& name);
  ~TcpClient();  // force out-line dtor, for scoped_ptr members.

  void connect();
  void disconnect();
  void stop();

  TcpConnectionPtr connection() 
  {
    ScopedMutex lock(&_mutex);
    return _connection;
  }

  EventLoop* getLoop() const { return _loop; }
  bool retry() const;
  void enableRetry() { _retry = true; }

  /// Set connection callback.
  /// Not thread safe.
  void setConnectionCallback(const ConnectionCallback& cb)
  { _connectionCallback = cb; }

  /// Set message callback.
  /// Not thread safe.
  void setMessageCallback(const MessageCallback& cb)
  { _messageCallback = cb; }

  /// Set write complete callback.
  /// Not thread safe.
  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { _writeCompleteCallback = cb; }

 private:
  /// Not thread safe, but in loop
  void newConnection(int sockfd);
  /// Not thread safe, but in loop
  void removeConnection(const TcpConnectionPtr& conn);

  EventLoop* _loop;
  AddrInet _serveraddr;
  ConnectorPtr _connector; // avoid revealing Connector
  const string _name;
  ConnectionCallback _connectionCallback;
  MessageCallback _messageCallback;
  WriteCompleteCallback _writeCompleteCallback;
  bool _retry;   // atomic
  bool _connect; // atomic
  // always in loop thread
  int _nextConnId;
  Mutex _mutex;
  TcpConnectionPtr _connection; // @GuardedBy mutex_
};
}
}

#endif
