#ifndef DRILL_NET_TCPSERVER_H_
#define DRILL_NET_TCPSERVER_H_

#include <drill/common/atomic.h>
#include <drill/net/tcp_connection.h>
#include <drill/net/call_back.h>
#include <drill/net/addr_inet.h>
#include <drill/common/noncopyable.h>

#include <functional>
#include <unordered_map>
#include <memory>
#include <string>

using namespace std;
using namespace drill::common;
using namespace drill::event;
namespace drill {

namespace event {
   class EventLoop;
   class EventLoopThreadPool;

}

namespace net {
class Acceptor;

///
/// TCP server, supports single-threaded and thread-pool models.
///
/// This is an interface class, so don't expose too much details.
class TcpServer : noncopyable
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;
  enum Option
  {
    kNoReusePort,
    kReusePort,
  };

  //TcpServer(EventLoop* loop, const InetAddress& listenAddr);
  TcpServer(EventLoop* loop,
            AddrInet& listenAddr,
            const string& nameArg,
            Option option = kReusePort);
  ~TcpServer();  // force out-line dtor, for scoped_ptr members.

  const string& hostport() const { return _hostport; }
  const string& name() const { return _name; }
  EventLoop* getLoop() const { return _loop; }

  /// Set the number of threads for handling input.
  ///
  /// Always accepts new connection in loop's thread.
  /// Must be called before @c start
  /// @param numThreads
  /// - 0 means all I/O in loop's thread, no thread will created.
  ///   this is the default value.
  /// - 1 means all I/O in another thread.
  /// - N means a thread pool with N threads, new connections
  ///   are assigned on a round-robin basis.
  void setThreadNum(int numThreads);
  void setThreadInitCallback(const ThreadInitCallback& cb)
  { _threadInitCallback = cb; }
  /// valid after calling start()
  std::shared_ptr<EventLoopThreadPool> threadPool()
  { return _threadPool; }

  /// Starts the server if it's not listenning.
  ///
  /// It's harmless to call it multiple times.
  /// Thread safe.
  void start();

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
  void newConnection(int sockfd, const AddrInet& peerAddr);
  /// Thread safe.
  void removeConnection(const TcpConnectionPtr& conn);
  /// Not thread safe, but in loop
  void removeConnectionInLoop(const TcpConnectionPtr& conn);

  typedef std::unordered_map<string, TcpConnectionPtr> ConnectionMap;
  EventLoop*                             _loop;  // the acceptor loop
  const string                           _hostport;
  const string                           _name;
  std::unique_ptr<Acceptor>              _acceptor; // avoid revealing Acceptor
  std::shared_ptr<EventLoopThreadPool>   _threadPool;
  ConnectionCallback                     _connectionCallback;
  MessageCallback                        _messageCallback;
  WriteCompleteCallback                  _writeCompleteCallback;
  ThreadInitCallback                     _threadInitCallback;
  AtomicInt32                            _started;
  // always in loop thread
  int                                    _nextConnId;
  ConnectionMap                          _connections;
};


}
}

#endif
