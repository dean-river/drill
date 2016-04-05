#ifndef DRILL_EVENT_EVENTLOOPTHREAD_H_
#define DRILL_EVENT_EVENTLOOPTHREAD_H_

#include <drill/common/noncopyable.h>
#include <drill/common/mutex.h>
#include <drill/common/cond.h>
#include <drill/common/thread.h>
#include <functional>
#include <string>

namespace drill {
namespace event {

class EventLoop;

class EventLoopThread : public drill::common::noncopyable 
{
 public:
  typedef std::function<void(EventLoop*)> ThreadInitCallback;

  EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                            const std::string &name = std::string());
  ~EventLoopThread();
  EventLoop* startLoop();
  void threadFunc();

 private:
  
  
  EventLoop                        *_loop;
  bool                              _exiting;
  drill::common::Mutex              _mutex;
  drill::common::Cond               _cond;
  drill::common::Thread             _thread;
  ThreadInitCallback                _callback;
};

}
}
#endif