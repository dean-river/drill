#include <drill/http/http_server.h>
#include <drill/http/http_request.h>
#include <drill/http/http_response.h>
#include <drill/common/logger.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>
#include <drill/ext/json/document.h>
#include <drill/common/any.h>
#include <string>

using namespace drill::net;
using namespace drill::event;
using namespace drill::http;



class LiveServer{
public:
	LiveServer(EventLoop *loop, AddrInet& listenAddr, const std::string& name, int threadnum);

	void start() { _server.start(); }

private:
	void onRequest(const HttpRequest& req, HttpResponse* resp);

private:
	HttpServer _server;

};


