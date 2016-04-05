#ifndef DRILL_EVENT_POLLER_H_
#define DRILL_EVENT_POLLER_H_

#include <drill/common/time.h>
#include <drill/event/event_loop.h>
#include <unordered_map>
#include <vector>


struct epoll_event;

namespace drill {
namespace event {

class Channel;

class Poller
{
 public:
  typedef std::vector<Channel*> ChannelList;

  Poller(EventLoop* loop);
  virtual ~Poller();

  /// Polls the I/O events.
  /// Must be called in the loop thread.
  drill::common::Time poll(int timeoutMs, ChannelList* activeChannels);

  /// Changes the interested I/O events.
  /// Must be called in the loop thread.
  void updateChannel(Channel* channel);

  /// Remove the channel, when it destructs.
  /// Must be called in the loop thread.
  void removeChannel(Channel* channel);

  bool hasChannel(Channel* channel) const;

  void assertInLoopThread() const
  {
    _ownerLoop->assertInLoopThread();
  }

 protected:
  typedef std::unordered_map<int, Channel*> ChannelMap;
  ChannelMap _channels;

 private:
  EventLoop* _ownerLoop;
  static const int kInitEventListSize = 16;

  void fillActiveChannels(int numEvents,
                          ChannelList* activeChannels) const;
  void update(int operation, Channel* channel);

  typedef std::vector<struct epoll_event> EventList;

  int _epollfd;
  EventList _events;
};

}
}
#endif
