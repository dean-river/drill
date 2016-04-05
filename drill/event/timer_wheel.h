#ifndef DRILL_EVENT_TIMEWHEEL_H_
#define DRILL_EVENT_TIMEWHEEL_H_

#include <drill/common/noncopyable.h>
#include <drill/event/timer_task.h>
#include <drill/event/timer_id.h>
#include <functional>
#include <memory>
#include <vector>
#include <cstddef>
#include <cstdint>

namespace drill {
namespace event {

class EventLoop;

struct  TimerWheelHander{
    std::shared_ptr<TimerTask> ptr; 
    void release()
    {
        if(ptr) {
            ptr.reset();
        }
    }

    bool valid()
    {
        return ptr ? true : false;
    }
   
};

class TimerWheel :public drill::common::noncopyable {
public:
    static const int64_t kDefaultNum = 60;
    typedef std::vector<std::weak_ptr<TimerTask> > TimerWheelList;
public:
    /*
        numb must be > 0;
        */
    TimerWheel(EventLoop *loop, int64_t numb = kDefaultNum);

    ~TimerWheel();
    
    TimerWheelHander addTimer(const TimerCallback&factor,
                                    int64_t delaySec,
                                    bool repeat
                                    );

    void start();
    void stop();
private:
    void onTimer();
    void addTimerInLoop(const std::shared_ptr<TimerTask> &task);
    void stopInLoop(); 
private:
    size_t _numBuckets;
    size_t _index;
    std::vector<TimerWheelList > _buffer;
    EventLoop   *_loop;
    TimerId      _timerid;
    bool         _stoped;
    
};

}
}
#endif