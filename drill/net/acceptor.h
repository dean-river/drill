#ifndef DRILL_NET_ACCEPTOR_H_
#define DRILL_NET_ACCEPTOR_H_
#include <drill/event/channel.h>
#include <drill/net/socket.h>

#include <functional>
#include <drill/common/noncopyable.h>


using namespace drill::event;

namespace drill {

namespace event {
class EventLoop;
}

namespace net {
class AddrInet;


class Acceptor : drill::common::noncopyable
{
 public:
  typedef std::function<void (int sockfd,
                                 AddrInet&)> NewConnectionCallback;

  Acceptor(EventLoop* loop,  AddrInet& listenAddr, bool reuseport );
  ~Acceptor();

  void setNewConnectionCallback(const NewConnectionCallback& cb)
  { _newConnectionCallback = cb; }

  bool listenning() const { return _listenning; }
  void listen();

 private:
  void handleRead();

  EventLoop* _loop;
  Socket _acceptSocket;
  Channel _acceptChannel;
  NewConnectionCallback _newConnectionCallback;
  bool _listenning;
  int _idleFd;
};

}
}


#endif
