#ifndef WALLE_NET_CONNECTION_H_
#define WALLE_NET_CONNECTION_H_

#include <drill/net/call_back.h>
#include <drill/common/buffer.h>
#include <drill/net/addr_inet.h>
#include <drill/common/any.h>
#include <drill/common/noncopyable.h>
#include <drill/common/slice.h>

#include <memory.h>
#include <drill/event/timer_wheel.h>
#include <string>

// struct tcp_info is in <netinet/tcp.h>
struct tcp_info;
using namespace drill::common;
using namespace drill::event;

namespace drill {
namespace event {
    
    class Channel;
    class EventLoop;

  }
namespace net {

class Socket;

///
/// TCP connection, for both client and server usage.
///
/// This is an interface class, so don't expose too much details.
class TcpConnection : drill::common::noncopyable,
                      public std::enable_shared_from_this<TcpConnection>
{
 public:
  /// Constructs a TcpConnection with a connected sockfd
  ///
  /// User should not create this object.
  TcpConnection(EventLoop *loop,
                const std::string &name,
                int sockfd,
                const AddrInet &localAddr,
                const AddrInet &peerAddr);
  ~TcpConnection();

  EventLoop* getLoop() const { return _loop; }
  const string& name() const { return _name; }
  const AddrInet& localAddress() const { return _localAddr; }
  const AddrInet& peerAddress() const { return _peerAddr; }
  bool connected() const { return _state == kConnected; }
  // return true if success.
  bool getTcpInfo(struct tcp_info*) const;
  string getTcpInfoString() const;

  void send(const void* message, int len);
  void send(const Slice &message);
  
  void send(Buffer* message);  // this one will swap data
  void shutdown();
  void forceClose();
  void forceCloseWithDelay(int64_t seconds);
  void setTcpNoDelay(bool on);

  void setContext(const any& context)
  { _context = context; }

  const any& getContext() const
  { return _context; }

  any* getMutableContext()
  { return &_context; }

  void setConnectionCallback(const ConnectionCallback& cb)
  { _connectionCallback = cb; }

  void setMessageCallback(const MessageCallback& cb)
  { _messageCallback = cb; }

  void setWriteCompleteCallback(const WriteCompleteCallback& cb)
  { _writeCompleteCallback = cb; }

  void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t 
highWaterMark)
  { _highWaterMarkCallback = cb; _highWaterMark = highWaterMark; }

  /// Advanced interface
  Buffer* inputBuffer()
  { return &_inputBuffer; }

  Buffer* outputBuffer()
  { return &_outputBuffer; }

  /// Internal use only.
  void setCloseCallback(const CloseCallback& cb)
  { _closeCallback = cb; }

  // called when TcpServer accepts a new connection
  void connectEstablished();   // should be called only once
  // called when TcpServer has removed me from its map
  void connectDestroyed();  // should be called only once
  void enableTimerOut(int64_t secs) ;
  void disableTimerOut();
  bool timerSet() { return _timerWheelHandler.valid(); }
 private:
  enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
  void handleRead(Time receiveTime);
  void handleWrite();
  void handleClose();
  void handleError();
  // void sendInLoop(string&& message);
  void sendInLoop(const Slice& message);
  void sendInLoop(const void* message, size_t len);
  void sendPiceInLoop(const Slice& message);
  void shutdownInLoop();
  // void shutdownAndForceCloseInLoop(double seconds);
  void forceCloseInLoop();
  void setState(StateE s) { _state = s; }
  void onTimerOut();

  Time                       _lastactive;
  int64_t                    _timerOutSec;
  EventLoop*                 _loop;
  const string               _name;
  StateE                     _state; 
  std::unique_ptr<Socket>    _socket;
  std::unique_ptr<Channel>   _channel;
  ConnectionCallback         _connectionCallback;
  MessageCallback            _messageCallback;
  WriteCompleteCallback      _writeCompleteCallback;
  HighWaterMarkCallback      _highWaterMarkCallback;
  CloseCallback              _closeCallback;
  Buffer                     _inputBuffer;
  Buffer                     _outputBuffer;
  any                       _context;
  AddrInet                   _localAddr;
  AddrInet                   _peerAddr;
  size_t                     _highWaterMark;
  TimerWheelHander           _timerWheelHandler;
};

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;


}
}
#endif
