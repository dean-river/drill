#ifndef DRILL_HTTP_URI_H_
#define DRILL_HTTP_URI_H_
#include <string>
#include <drill/common/slice.h>

using namespace drill::common;

namespace drill {
namespace http {

struct  Uri {
  Uri():secure(false), valid(false) {}
  Uri(const Slice &line):secure(false), valid(false)
  {
    Slice uri(line);
    if(line.size() == 0) {
      return;
    }
    size_t uri_len = line.size(); 

    if(uri.starts_with("http://")) {
      scheme =  "http";
      uri.remove_prefix(7);
    } else if(uri.starts_with("https://")) {
      scheme =  "https";
      secure = true;
      uri.remove_prefix(8);
    }

    // 0 need continue
    // 1  ":"
    // 2 "/" or end
    int state = 0;
    const char *it = uri.data();
    const char *end = uri.data() + uri.size();

    while (state == 0) {

      if(it == end) {
        state = 2;
        break;
      } else if(*it == '/') {
        state = 2;
      } else if(*it == ':') {
        state = 1;
      } else {
        host += *it;
      }
      ++it;
    }

    while (state == 1) {
      if(it == end) {
        state = 2;
        break;
      } else if(*it == '/') {
        state = 2;
      }  else {
        port += *it;
      }
      ++it;
    }

    uri.remove_prefix(it - uri.data());
    //location
    location += "/";
    location.append(uri.data(), uri.size());
    valid = true;
    
  }

  Uri(bool s, std::string const & h, 
    const std::string &p,
    std::string const & l)
    :secure(s),
    valid(true),
    scheme(s?"https":"http"),
    host(h),
    port(p),
    location(l)
  {

  }

    Uri(bool s, std::string const & h, 
    std::string const & l)
    :secure(s),
    valid(true),
    scheme(s?"https":"http"),
    host(h),
    port(s?"443","80"),
    location(l)
  {

  }

    
  bool        secure;
  bool        valid;
  std::string scheme;
  std::string host;
  std::string port;
  std::string location;

};
}
}
#endif