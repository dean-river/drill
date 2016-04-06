#ifndef WALLE_LOGFILE_H_
#define WALLE_LOGFILE_H_
#include <drill/common/sequence_write_file.h>
#include <drill/common/time.h>
#include <drill/common/mutex.h>


#include <string>
#include <cstddef>
#include <memory>

using std::string;
using drill::common::Time;
using drill::common::SequenceWriteFile;
using drill::common::Mutex;

namespace drill {
namespace service {
	
class LogFile{
  public:
    LogFile(const string & base,
                const size_t rollsize ,
                bool threadsafe = true,
                const int & flushinterval = 3,
                const int checksize=4096);
    ~LogFile();
    void append(const char * line, size_t len);
    void append(const string & logline);
    bool roll();
    void flush();
    
  private:
    void append_unlocked(const char* logline, size_t len);
    string getLogFileName(const Time &t);
	
    const string _basename;
    const size_t _rollsize;
    const int    _flushstep;
    const int    _checkN;
    int          _count;

    std::unique_ptr<Mutex> _mutex;
    Time                    _lastroll;
    Time                    _lastflush;
    std::unique_ptr<SequenceWriteFile> _file;
    
        
};
}
}
#endif
