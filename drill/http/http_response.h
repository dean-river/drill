#ifndef DRILL_HTTP_RESPONSE_H_
#define DRILL_HTTP_RESPONSE_H_
#include <drill/http/constants.h>
#include <string>
#include <map>
#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <memory>

using namespace std;

namespace drill {
namespace http {

class HttpResponse {
    public:
      HttpResponse();
      ~HttpResponse();
      void recvStatLine();
      void recvHeader();
      void recvBody();

      bool getAll();
      bool isvalid();
      void setValid(bool valid);
      size_t parseResponse(const char* buff, size_t len);

      
		http_parse_status parseStat;
		size_t  need_do_size;
		size_t  had_do_size;		
        int     httpcode;
        string  version;
        string  httpStatline;
        Dict    header;
        string  body;

    private:
      size_t    parseStatLine(const char* buff, size_t len);
      size_t    parseHeaders(const char* buff, size_t len);
      size_t    parseBody(const char* buff, size_t len);
	  void      preParseBody();
};

typedef std::shared_ptr<HttpResponse> HttpResponsePtr;
}

}
#endif