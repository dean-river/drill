#ifndef DRILL_HTTP_HTTP_CONTEXT_H_
#define DRILL_HTTP_HTTP_CONTEXT_H_
#include <drill/net/tcp_connection.h>
#include <drill/common/any.h>
#include <drill/http/constants.h> 
#include <functional>

using namespace drill::net;
using namespace drill::common;
namespace drill {
namespace http {
  
class HttpSession;

typedef  std::function<void(HttpSession*)> HttpCallBack;

class HttpSession {
  public:
    HttpSession();

  private:
     void onConnection(const TcpConnectionPtr& conn);
     void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf, Time receiveTime);
  private:
    http_process_status _status;
    HttpRequestPtr      _request;
    HttpResponsePtr     _response;
    any                 _context;
    HttpCallBack        _onHost;
    HttpCallBack        _onMathod;
    HttpCallBack        _onUri;
    HttpCallBack        _onVersion;
    HttpCallBack        _onHeader;
    HttpCallBack        _onBody;
    HttpCallBack        _onResponse;
    HttpCallBack        _onLog;
    
  
};

}
}

#endif
