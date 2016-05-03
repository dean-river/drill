#include <drill/httpxx/http_server.h>
#include <drill/common/logger.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>
#include <drill/ext/json/document.h>
#include <drill/common/any.h>
#include <string>

using namespace drill::net;
using namespace drill::event;
using namespace drill::httpxx;



class LiveServerAsync{
public:
	LiveServerAsync(EventLoop *loop, AddrInet& listenAddr, const std::string& name, int threadnum);

	void start() { _server.start(); }

private:
	bool builder(HttpServerSession *hss);
	bool clear(HttpServerSession *hss);

private:
	HttpServer _server;

};


