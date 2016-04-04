#ifndef DRILL_COMMON_LOGSTREAM_H_
#define DRILL_COMMON_LOGSTREAM_H_
#include <drill/common/fixed_buffer.h>
#include <drill/common/string.h>
#include <drill/common/slice.h>

namespace drill {

namespace common {
    
class LogStream{
public:
    typedef LogStream self;
    typedef FixedBuffer<ksmallbuffer> Buffer;
public:
    LogStream(){}
    ~LogStream(){}
    self& operator<<(bool v)
    {
        _buffer.append(v ? "1" : "0", 1);
        return *this;
    }

    self& operator<<(short);
    self& operator<<(unsigned short);
    self& operator<<(int);
    self& operator<<(unsigned int);
    self& operator<<(long);
    self& operator<<(unsigned long);
    self& operator<<(long long);
    self& operator<<(unsigned long long);
    //self& operator<<(const Console::TextColor& color);
    self& operator<<(const void*);

    self& operator<<(float v)
    {
            *this << static_cast<double>(v);
        return *this;
    }

    self& operator<<(double);
  // self& operator<<(long double);

  self& operator<<(char v)
  {
        _buffer.append(&v, 1);
    return *this;
  }

  self& operator<<(const char* v)
  {
        _buffer.append(v, strlen(v));
    return *this;
  }

  self& operator<<(const std::string& v)
  {
      _buffer.append(v.c_str(), v.size());
    return *this;
  }

  self& operator<<(const Slice& v)
  {
        _buffer.append(v.data(), v.size());
    return *this;
  }

  void append(const char* data, size_t len)
  {
     _buffer.append(data, len);
  }
  const Buffer& buffer() const { return _buffer; }
  void resetBuffer() { _buffer.reset(); }
private:
    template <typename T>
    void appendInt(T value) 
    {
        std::string result = to_str(value);
        if(_buffer.avail() > result.length()) {
            _buffer.append(result.c_str(),result.length());
        }
        
    }
    Buffer _buffer;
};
}
}
#endif
