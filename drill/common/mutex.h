#ifndef DRILL_COMMON_MUTEX_H_
#define DRILL_COMMON_MUTEX_H_
#include <cstdint>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <cassert>
#include <cmath>

namespace drill{
namespace common {
  enum Type {
    FAST,                ///< no operation
    ERRORCHECK,          ///< check error
    RECURSIVE            ///< allow recursive locking
  };
class Mutex{

public:
      friend class Cond;
public:

  explicit Mutex();

  explicit Mutex(Type type);

  ~Mutex();

  void lock();

  bool lock_try();

  bool lock_try(int64_t usec);

  void unlock();
private:
  pthread_mutex_t _mutex;
};
}
}
#endif