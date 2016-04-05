#ifndef DRILL_COMMON_TIMERID_H_
#define DRILL_COMMON_TIMERID_H_

namespace drill {
namespace event {
	
class TimerTask;

class TimerId {
 public:
  TimerId()
    : _task(NULL),
      _sequence(0)
  {
  }

  TimerId(TimerTask* task, int64_t seq)
    : _task(task),
      _sequence(seq)
  {
  }


  friend class Timer;

 private:
  TimerTask* _task;
  int64_t _sequence;
};
}
}
#endif