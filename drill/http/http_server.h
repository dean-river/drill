#ifndef WALLE_HTTPSERVER_H_
#define WALLE_HTTPSERVER_H_

#include <drill/net/tcp_server.h>
#include <drill/net/addr_inet.h>
#include <drill/common/time.h>
#include <drill/common/buffer.h>
#include <drill/common/noncopyable.h>
#include <drill/http/http_response.h>
#include <drill/http/http_request.h>
#include <functional>


using namespace drill::common;
using namespace drill::net;
using namespace drill::event;

namespace drill {
namespace http {

class HttpServer 
{
 public:
  typedef std::function<void (const HttpRequest&,
                                HttpResponse*)> HttpCallback;

  HttpServer(EventLoop* loop,
             AddrInet& listenAddr,
             const string& name,
             TcpServer::Option option = TcpServer::kNoReusePort);

  ~HttpServer();  // force out-line dtor, for scoped_ptr members.

  EventLoop* getLoop() const { return _server.getLoop(); }

  /// Not thread safe, callback be registered before calling start().
  void setHttpCallback(const HttpCallback& cb)
  {
    _httpCallback = cb;
  }

  void setThreadNum(int numThreads)
  {
    _server.setThreadNum(numThreads);
  }

  void start();

 private:
  void onConnection(const TcpConnectionPtr& conn);
  void onMessage(const TcpConnectionPtr& conn,
                 Buffer* buf, Time receiveTime);
  void onRequest(const TcpConnectionPtr&, const HttpRequest&);

  TcpServer _server;
  HttpCallback _httpCallback;
};

}
}


#endif
