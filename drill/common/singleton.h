#ifndef DRILL_COMMON_SINGLETON_H_
#define DRILL_COMMON_SINGLETON_H_

#include <cstdlib>
#include <drill/common/mutex.h>
#include <drill/common/scoped_lock.h>

namespace drill {
namespace common {

template <typename Instance, bool destroy_on_exit = true>
class Singleton
{
    typedef Instance instance_type;
    typedef  instance_type* instance_pointer;
    typedef volatile instance_pointer volatile_instance_pointer;

    static volatile_instance_pointer _instance;

    static instance_pointer CreateInstance();
    static void DestroyInstance();

public:
    Singleton() {}

    //! non-copyable: delete copy-constructor
    
    inline static instance_pointer GetInstance() {
        if (!_instance)
            return CreateInstance();

        return _instance;
    }
private:
    Singleton(const Singleton&);;
    //! non-copyable: delete assignment operator
    Singleton& operator = (const Singleton&);
};

template <typename Instance, bool destroy_on_exit>
typename Singleton<Instance, destroy_on_exit>::instance_pointer
Singleton<Instance, destroy_on_exit>::CreateInstance() {
    static Mutex create_mutex;
    ScopedMutex lock(&create_mutex);
    if (!_instance) {
        _instance = new instance_type();
        if (destroy_on_exit)
            atexit(DestroyInstance);
    }
    return _instance;
}

template <typename Instance, bool destroy_on_exit>
void Singleton<Instance, destroy_on_exit>::DestroyInstance() 
{
    instance_pointer inst = _instance;
    // instance = nullptr;
    _instance = reinterpret_cast<instance_pointer>(size_t(-1));     // bomb if used again
    delete inst;
}

template <typename Instance, bool destroy_on_exit>
typename Singleton<Instance, destroy_on_exit>::volatile_instance_pointer Singleton<Instance, destroy_on_exit>::_instance = NULL;

}
} 
#endif 
