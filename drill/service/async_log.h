#ifndef DRILL_SERVICE_ASYNC_LOG_H_
#define DRILL_SERVICE_ASYNC_LOG_H_

#include <drill/common/blocked_queue.h>
#include <drill/common/fixed_buffer.h>
#include <drill/common/count_down.h>
#include <drill/common/mutex.h>
#include <drill/common/thread.h>
#include <drill/common/noncopyable.h>

#include <functional>
#include <vector>

using namespace drill::common;

namespace drill {
namespace service {

class LogFile;
class AsyncLogging :noncopyable
{
 public:

  AsyncLogging(const string& basename,
               size_t rollSize,
               int flushInterval = 3);

  ~AsyncLogging()
  {
    if (_isrunning)
    {
      stop();
    }
  }

  void append(const char* logline, size_t len);
  void waitStart() 
  {
    if (_isrunning)
        return;
    _isrunning = true;
    _th.start();
    _latch.wait();
  }
  void stop()
  {
    if (_isrunning) {
        _isrunning = false;
        _cond.signal();
        _th.join();
    }
  }
  void run();


 private:
     void flushall(LogFile* output);

  // declare but not define, prevent compiler-synthesized functions
  AsyncLogging(const AsyncLogging&);  // ptr_container
  void operator=(const AsyncLogging&);  // ptr_container

  typedef FixedBuffer<klargebuffer> Buffer;
  typedef vector<Buffer*> BufferVector;
 
  const int     _flushInterval;
  bool          _isrunning;
  string        _basename;
  size_t        _rollSize;
  CountDown     _latch;
  Mutex         _mutex;
  Cond          _cond;
  Buffer       *_currentBuffer;
  Buffer       *_nextBuffer;
  BufferVector _buffers;
  Thread       _th;
};

}

}
#endif

