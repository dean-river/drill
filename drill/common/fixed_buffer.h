#ifndef DRILL_COMMON_FIXED_BUFFER_H_
#define DRILL_COMMON_FIXED_BUFFER_H_

#include <cstddef>
#include <string>
#include <cstring>

namespace drill {
namespace common {
    static const size_t ktinybuffer = 1024;
    static const size_t kminibuffer = 1024;
    static const size_t ksmallbuffer = 4096;
    static const size_t kmediumbuffer = 1024 * 128;
    static const size_t klargebuffer = 1024*1024;
    static const size_t khugebuffer = 10*1024*1024;

template <int SIZE>
class FixedBuffer{
public:
    FixedBuffer():_cur(_data){}
    ~FixedBuffer() {}
    bool append(const char *buf, const size_t len)
    {
        if (len < avail() ) {
            memcpy(_cur, buf, len);
            _cur += len;
            return true;
        }
        return false;
    }
    char * current() { return _cur; };
    const char *data() const{ return _data; }
    size_t size() const{return _cur - _data; }

    size_t avail() { return end() - _cur; }

    void add(size_t len) { _cur += len;}
    void reset(){_cur = _data; }
    void bzero() { ::memset(_data, 0, sizeof(_data)); }

    const char *cstr() { *_cur = '\0'; return _data;}

    std::string toString();

    

private:
    const char *end() { return _data + sizeof(_data); }
    char    _data[SIZE];
    char   *_cur;
};

}
}
#endif
