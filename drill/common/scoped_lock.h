#ifndef DRILL_COMMON_SCOPEDLOCK_H_
#define DRILL_COMMON_SCOPEDLOCK_H_
#include <assert.h>
#include <drill/common/mutex.h>

namespace drill {
namespace common {

template <typename LOCK>

class ScopedLock {
public:
    explicit ScopedLock(LOCK* lock)
    {
        _lock = lock;
        if (_lock)
            _lock->lock();
    }
    ~ScopedLock()
    {
      if (_lock)
        _lock->unlock();  
    }
private:
    LOCK* _lock;
};

typedef ScopedLock<Mutex> ScopedMutex;

}
}
#endif
