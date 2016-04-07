#ifndef DRILL_HTTP_HTTPREQUEST_H_
#define DRILL_HTTP_HTTPREQUEST_H_
#include <drill/http/constants.h>
#include <drill/http/uri.h>
#include <string>
#include <unordered_map>
#include <cstdint>

namespace drill {
namespace http {

class HttpRequest {
  public:

  public:
    typedef std::unordered_map<string, string >  Dict;
    typedef Dict::iterator DictItr;
    
    string  method;
    string  version;
    Uri     uri;
    Dict    args;
    Dict    header;
    http_req_parse_status  parse_status;
    http_req_packet_status packet_status;
};


}
}


#endif