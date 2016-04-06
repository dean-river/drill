#ifndef WALLE_APPEND_FILE_H_
#define WALLE_APPEND_FILE_H_

#include <cstdint>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cstdlib>

using namespace std;
namespace drill {
namespace common {    
class SequenceWriteFile {
    public:
        static const size_t kTempBuffSize = 64 * 1024;
      SequenceWriteFile(const char* filename);
      SequenceWriteFile(const string& filename);
      ~SequenceWriteFile();
      void flush();
      bool append(const char* content, const size_t len);
      size_t writeSize();
    private:
       size_t unlockWrite(const char* content, const size_t len);
       FILE  *_fp;
       size_t _writesize;
       char   _buffer[kTempBuffSize];
       
       
};
}
}
#endif
