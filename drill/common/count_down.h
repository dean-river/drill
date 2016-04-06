#ifndef DRILL_COMMON_COUNTDOWN_H_
#define DRILL_COMMON_COUNTDOWN_H_

#include <drill/common/mutex.h>
#include <drill/common/cond.h>

namespace drill {

namespace common {

class CountDown {
    public:
        explicit CountDown(int c);
        ~CountDown();
        void down();
        void wait();
        int getCount();
   private:
        Mutex _mutex;
        Cond  _cond;
        int   _count;
        
};

}
}
#endif
