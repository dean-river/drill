#include <drill/common/count_down.h>
#include <drill/common/scoped_lock.h>

namespace drill {
namespace common {

CountDown::CountDown(int c):_mutex(),_cond(),_count(c)
{
	
}
CountDown::~CountDown()
{
}
void CountDown::down()
{
	ScopedMutex lock(&_mutex);
	_count--;
	if(_count == 0) {
		_cond.broadcast();
	}
}
void CountDown::wait()
{
	ScopedMutex lock(&_mutex);
	while (_count > 0 ) {
		_cond.wait(&_mutex);
	}
}
int CountDown::getCount()
{
	ScopedMutex lock(&_mutex);
	return _count;
}

}
}
