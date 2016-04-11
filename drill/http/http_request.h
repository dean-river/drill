#ifndef DRILL_HTTP_HTTPREQUEST_H_
#define DRILL_HTTP_HTTPREQUEST_H_
#include <drill/http/constants.h>
#include <drill/http/uri.h>
#include <string>
#include <cstdint>
#include <memory>

using std::string;

namespace drill {
namespace http {

class HttpRequest {
  public:
	HttpRequest();
	void recvRequestLine() { parse_status = e_header; }
	void recvheader() { parse_status = e_body; }

	void recvBody() { parse_status = g_all; }

	bool gotAll() { return parse_status == g_all; }
	
	bool error() { return parse_status == e_error; }
	
	size_t parseRequest(const char *data, size_t len);
	
public:

	http_parse_status      parse_status;
	http_body_encode       encoding;
	size_t      need_do_size;
	size_t      had_do_size;
  string  method;
  string  version;
  Uri     uri;
  Dict    args;
  Dict    header;
	std::string body;
	
private:

    size_t    parseRequestLine(const char* buff, size_t len);
	  size_t    parseHeaders(const char* buff, size_t len);
	  void      preParseBody();
	  bool      parseArgs(const char* buff, size_t len);
	  size_t    parseBody(const char* buff, size_t len);

};

typedef std::shared_ptr<HttpRequest> HttpRequestPtr;
}
}


#endif