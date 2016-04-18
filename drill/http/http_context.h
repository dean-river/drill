#ifndef DRILL_HTTP_HTTPCONTEXT_H_
#define DRILL_HTTP_HTTPCONTEXT_H_
#include <drill/http/http_constants.h>
#include <drill/http/http_request.h>

namespace drill {
namespace http{

class HttpContext
{
 public:

  HttpContext()
    : _state(kPRequestLine)
  {
  }

  // default copy-ctor, dtor and assignment are fine

  bool expectRequestLine() const
  { return _state == kPRequestLine; }

  bool expectHeaders() const
  { return _state == kPHeader; }

  bool expectBody() const
  { return _state == kPBody; }

  bool gotAll() const
  { return _state == kPGotAll; }

  void receiveRequestLine()
  { _state = kPHeader; }

  void receiveHeaders()
  { _state = kPBody; }
  void receiveBody()
  {
	_state =kPGotAll;
  }

  void reset()
  {
    _state = kPRequestLine;
    HttpRequest dummy;
    _request.swap(dummy);
  }

  const HttpRequest& request() const
  { return _request; }

  HttpRequest& request()
  { return _request; }

 private:
  httpParseStatus _state;
  HttpRequest _request;
};

}
}
#endif