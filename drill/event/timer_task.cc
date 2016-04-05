#include <drill/event/timer_task.h>


namespace drill {
namespace event {
	
drill::common::AtomicInt64 TimerTask::_gTimerNum;

void TimerTask::restart(const drill::common::Time &now)
{
  if (_repeat)
  {
    _expiration = now + _interval;
  }
  else
  {
    _expiration = drill::common::Time();
  }
}
}

}
