#ifndef DRILL_EVENT_TIMER_H_
#define DRILL_EVENT_TIMER_H_

#include <drill/event/channel.h>
#include <drill/event/timer_id.h>
#include <drill/event/timer_task.h>
#include <drill/common/time.h>
#include <set>
#include <vector>
#include <stdint.h>

namespace drill {
namespace event {

class EventLoop;
class TimerTask;
class TimerId;

///
/// A best efforts timer queue.
/// No guarantee that the callback will be on time.
///
class Timer
{
 public:
  Timer(EventLoop* loop);
  ~Timer();

  ///
  /// Schedules the callback to be run at given time,
  /// repeats if @c interval > 0.0.
  ///
  /// Must be thread safe. Usually be called from other threads.
  TimerId addTimer(const TimerCallback& cb,
                   drill::common::Time when,
                   int64_t interval);

  void cancel(TimerId timerId);
private:
  Timer& operator=(const Timer&);
  Timer(const Timer&);
 private:

  // FIXME: use unique_ptr<Timer> instead of raw pointers.
  typedef std::pair<drill::common::Time, TimerTask*> Entry;
  typedef std::set<Entry> TaskList;
  typedef std::pair<TimerTask*, int64_t> ActiveTask;
  typedef std::set<ActiveTask> ActiveTaskSet;

  void addTimerInLoop(TimerTask* task);
  void cancelInLoop(TimerId timerId);
  // called when timerfd alarms
  void handleRead();
  // move out all expired timers
  std::vector<Entry> getExpired(drill::common::Time now);
  void reset(const std::vector<Entry>& expired, drill::common::Time now);

  bool insert(TimerTask* task);

  EventLoop* _loop;
  const int _timerfd;
  Channel _timerfdChannel;
  // Timer list sorted by expiration
  TaskList _tasks;

  // for cancel()
  ActiveTaskSet _activeTasks;
  bool _callingExpiredTasks; /* atomic */
  ActiveTaskSet _cancelingTasks;
};

}
}
#endif