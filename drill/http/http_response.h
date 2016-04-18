#ifndef WALLE_HTTPREPONSE_H_
#define WALLE_HTTPREPONSE_H_
#include <drill/http/http_constants.h>
#include <drill/common/buffer.h>
#include <drill/common/string.h>


#include <string>
#include <map>


using namespace drill::common;
using namespace std;
namespace drill  {
namespace http {
	
class HttpResponse 
{
 public:
  explicit HttpResponse(bool close)
    : _statusCode(uninitialized),
      _closeConnection(close)
  {
  }

  void setStatusCode(HttpStatusCode code)
  { _statusCode = code; }

  void setStatusMessage(const string& message)
  { _statusMessage = message; }

  void setCloseConnection(bool on)
  { _closeConnection = on; }

  bool closeConnection() const
  { return _closeConnection; }

  void setContentType(const string& contentType)
  { addHeader("Content-Type", contentType); }

  void setContentLeng(int64_t n) 
  {
    string len = to_str(n);
    addHeader("Content-Length",len);
    
  }

  // FIXME: replace string with Slice
  void addHeader(const string& key, const string& value)
  { _headers[key] = value; }

  void setBody(const string& body)
  {
    _body = body;
    setContentLeng(_body.size());
  }

  void appendToBuffer(Buffer* output) const;

 private:
  std::map<string, string> _headers;
  HttpStatusCode           _statusCode;
  string                   _statusMessage;
  bool                     _closeConnection;
  string                   _body;
};

}
}
#endif
