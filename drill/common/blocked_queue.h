#ifndef DRILL_COMMON_BLOCKQUEUE_H_
#define DRILL_COMMON_BLOCKQUEUE_H_

#include <drill/common/cond.h>
#include <drill/common/mutex.h>
#include <drill/common/scoped_lock.h>
#include <drill/common/noncopyable.h>
#include <deque>
#include <assert.h>

namespace drill {
namespace common {

template<typename T>
    
class BlockQueue :noncopyable
{
 public:
  BlockQueue()
    : _mutex(),
      _notEmpty(),
      _queue()
  {
  }

  void put(const T& x)
  {
    ScopedMutex lock(&_mutex);
    _queue.push_back(x);
    _notEmpty.signal();
  }

  T take()
  {
    ScopedMutex lock(&_mutex);
    while (_queue.empty())
    {
      _notEmpty.wait(&_mutex);
    }
    assert(!_queue.empty());
    T front(_queue.front());
    _queue.pop_front();
    return front;
  }

  size_t size() const
  {
    ScopedMutex lock(&_mutex);
    return _queue.size();
  }

 private:
  Mutex             _mutex;
  Cond              _notEmpty;
  std::deque<T>     _queue;
};

}
}

#endif
