#ifndef WALLE_THREAD_THREADPOOL_H_
#define WALLE_THREAD_THREADPOOL_H_

#include <drill/common/cond.h>
#include <drill/common/mutex.h>
#include <drill/common/thread.h>

#include <functional>
#include <drill/common/noncopyable.h>
#include <deque>
#include <string>
#include <vector>

using std::string;

namespace drill  {
namespace common {

class ThreadPool : noncopyable {
 public:
  typedef std::function<void ()> Task;

  explicit ThreadPool(const string& nameArg = string("ThreadPool"));
  ~ThreadPool();

  // Must be called before start().
  void setMaxQueueSize(int maxSize) { _maxQueueSize = maxSize; }
  void setThreadInitCallback(const Task& cb)
  { _threadInitCallback = cb; }

  void start(int numThreads);
  void stop();

  const string& name() const
  { return _name; }

  size_t queueSize() const;

  // Could block if maxQueueSize > 0
  void run(const Task& f);
 private:
  bool isFull() const;
  void runInThread();
  Task take();

  mutable Mutex _mutex;
  Cond _notEmpty;
  Cond _notFull;
  string _name;
  Task _threadInitCallback;
  std::vector<Thread*> _threads;
  std::deque<Task> _queue;
  size_t _maxQueueSize;
  bool _running;
};

}
}
#endif
