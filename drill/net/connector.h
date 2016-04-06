#ifndef DRILL_NET_CONNECTOR_H_
#define DRILL_NET_CONNECTOR_H_

#include <drill/net/addr_inet.h>
#include <drill/common/noncopyable.h>
#include <drill/net/socket.h>
#include <memory>
#include <functional>

using namespace drill::common;

namespace drill {

namespace event {

class EventLoop;
class Channel;

}

namespace net {


using namespace drill::event;


class Connector : noncopyable,public std::enable_shared_from_this<Connector>
{
 public:
  typedef std::function<void (int sockfd)> NewConnectionCallback;

  Connector(EventLoop* loop, const AddrInet& serverAddr);
  ~Connector();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { _newConnectionCallback = cb; }

  void start();  // can be called in any thread
  void restart();  // must be called in loop thread
  void stop();  // can be called in any thread

  const AddrInet& serverAddress() const { return _serverAddr; }

 private:
  enum States { kDisconnected, kConnecting, kConnected };
  static const int kMaxRetryDelayMs = 30*1000;
  static const int kInitRetryDelayMs = 500;

  void setState(States s) { _state = s; }
  void startInLoop();
  void stopInLoop();
  void connect();
  void connecting(int sockfd);
  void handleWrite();
  void handleError();
  void retry(int sockfd);
  int removeAndResetChannel();
  void resetChannel();
  Socket                     _socket;
  EventLoop*                 _loop;
  AddrInet                   _serverAddr;
  bool                       _connect; // atomic
  States                     _state;  // FIXME: use atomic variable
  std::unique_ptr<Channel>   _channel;
  NewConnectionCallback      _newConnectionCallback;
  int                        _retryDelayMs;
};

}
}
#endif
