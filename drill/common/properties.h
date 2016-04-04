#ifndef DRILL_COMMON_PROPERTIES_H_
#define DRILL_COMMON_PROPERTIES_H_
  
#include <istream>
#include <string>
#include <map>

namespace drill {
namespace common {

class Properties {
  public  :
    // load the properties from a file
    int loadProperties(const char* fileName, char delimiter, bool verbose, bool multiline = false);
    // load the properties from an in-core buffer
    int loadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline = false);
    std::string getValue(std::string key, std::string def) const;
    const char* getValue(std::string key, const char* def) const;
    int getValue(std::string key, int def);
    long getValue(std::string key, long def);
    long long getValue(std::string key, long long def);
    uint64_t getValue(std::string key, uint64_t def);
    double getValue(std::string key, double def);   
    void setValue(const std::string key, const std::string value);
    void getList(std::string &outBuf, std::string linePrefix) const;
    Properties();
    Properties(const Properties &p);
    ~Properties();
	
  private :
    //Map that holds the (key,value) pairs
    std::map<std::string, std::string> * propmap; 

};

}
}
#endif 
