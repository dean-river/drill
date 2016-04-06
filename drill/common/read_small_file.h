#ifndef DRILL_READ_SMALL_FILE_H_
#define DRILL_READ_SMALL_FILE_H_
#include <cstdint>
#include <string>
#include <cstdio>
#include <cstddef>
#include <cstdlib>
using namespace std;

namespace drill{
namespace common {

class ReadSmallFile{
    public:
    static const size_t kmaxsmallreadsize = 128*1024;
    ReadSmallFile();
    ReadSmallFile(const string &filename);
    ReadSmallFile(const char* filename);
    ~ReadSmallFile();
    bool read(string *content, size_t maxlen);
    
    private:
       FILE   *_fp;
       char    _buff[kmaxsmallreadsize];
};  
}
}
#endif
