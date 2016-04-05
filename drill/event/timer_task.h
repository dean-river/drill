#ifndef DRILL_EVENT_TIMERTASK_H_
#define DRILL_EVENT_TIMERTASK_H_
#include <drill/common/time.h>
#include <drill/common/atomic.h>
#include <functional>
#include <cstdint>



namespace drill {
namespace event {

typedef std::function<void()> TimerCallback;

class TimerTask {
 public:
  TimerTask(const TimerCallback& cb, const drill::common::Time& when,
                const int64_t& interval)
    : _callback(cb),
      _expiration(when),
      _interval(interval),
      _repeat(interval > 0),
      _sequence(_gTimerNum.incrementAndGet())
  { }

 void run() const
  {
    _callback();
  }

  drill::common::Time& expiration()  { return _expiration; }
  bool repeat() const { return _repeat; }
  int64_t sequence() const { return _sequence; }

  void restart(const drill::common::Time &now);

  int64_t step() const { return _interval; }
  
  static int64_t numCreated() { return _gTimerNum.get(); }

 private:
  const TimerCallback _callback;
  drill::common::Time _expiration;
  const int64_t _interval;
  const bool _repeat;
  const int64_t _sequence;

  static drill::common::AtomicInt64 _gTimerNum;
};
}
}

#endif
