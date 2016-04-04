#ifndef WALLE_SYS_LOGGING_H_
#define WALLE_SYS_LOGGING_H_
#include <drill/common/log_stream.h>
#include <drill/common/time.h>

#pragma GCC diagnostic ignored "-Wtype-limits"

namespace drill {

namespace common {
	
class Logger
{
 public:
  enum LogLevel {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM_LOG_LEVELS
  };

  class SourceFile
  {
   public:
    template<int N>
    inline SourceFile(const char (&arr)[N])
      : _data(arr),
        _size(N-1)
    {
      const char* slash = strrchr(_data, '/'); // builtin function
      if (slash)
      {
        _data = slash + 1;
        _size -= _data - arr;
      }
    }

    explicit SourceFile(const char* filename)
      : _data(filename)
    {
      const char* slash = strrchr(filename, '/');
      if (slash)
      {
        _data = slash + 1;
      }
      _size = static_cast<int>(strlen(_data));
    }

    const char* _data;
    size_t      _size;
  };

  Logger(SourceFile file, int line);
  Logger(SourceFile file, int line, LogLevel level);
  Logger(SourceFile file, int line, LogLevel level, const char* func);
  Logger(SourceFile file, int line, bool toAbort);
  ~Logger();

  LogStream& stream() { return _impl._stream; }

  static LogLevel logLevel();
  static void setLogLevel(LogLevel level);


  typedef void (*OutputFunc)(const char* msg, size_t len);
  typedef void (*FlushFunc)();
  static void setOutput(OutputFunc);
  static void setFlush(FlushFunc);

 private:

class Impl
{
 public:
  typedef Logger::LogLevel LogLevel;
  Impl(LogLevel level, int old_errno, const SourceFile& file, int line);
  void formatTime();
  void finish();

  Time       _time;
  LogStream  _stream;
  LogLevel   _level;
  int        _line;
  SourceFile _basename;
};

  Impl       _impl;

};

extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel()
{
  return g_logLevel;
}



const char* strerror_tl(int savedErrno);

// Taken from glog/logging.h
//
// Check that the input is non NULL.  This very useful in constructor
// initializer lists.

#define CHECK_NOTNULL(val) \
  ::dlsys::CheckNotNull(__FILE__, __LINE__, "'" #val "' Must be non NULL", (val))

// A small helper for CHECK_NOTNULL().
template <typename T>
T* CheckNotNull(Logger::SourceFile file, int line, const char *names, T* ptr) {
  if (ptr == NULL) {
   Logger(file, line, Logger::FATAL).stream() << names;
  }
  return ptr;
}

}

}
#define LOG_TRACE if(drill::common::Logger::TRACE >= drill::common::Logger::logLevel()) \
    drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::TRACE, __func__).stream()
#define LOG_DEBUG if(drill::common::Logger::DEBUG >= drill::common::Logger::logLevel()) \
    drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::DEBUG, __func__).stream()
#define LOG_INFO if(drill::common::Logger::INFO >= drill::common::Logger::logLevel()) \
    drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::INFO,__func__).stream()
#define LOG_WARN if(drill::common::Logger::WARN >= drill::common::Logger::logLevel()) \
    drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::WARN,__func__).stream()
#define LOG_ERROR drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::ERROR, __func__).stream()
#define LOG_FATAL drill::common::Logger(__FILE__, __LINE__, drill::common::Logger::FATAL, __func__).stream()


#endif
