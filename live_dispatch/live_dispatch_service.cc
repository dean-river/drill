#include <live_dispatch/live_dispatch_service.h>
#include <drill/net/addr_inet.h>
#include <cstdlib>

using namespace drill::net;

LiveService::LiveService(): _server(NULL)
{

}
LiveService::~LiveService()
{

}
int LiveService::run()
{
	int thnum = _conf.getValue("thread_num",1);
	std::string servername = _conf.getValue("service_name","live_dispath");
	std::string addr = _conf.getValue("server_listen_addr","live_dispath");
	AddrInet serveraddr(addr.c_str());
	_serverBaseloop = _th.startLoop();
	if(!_serverBaseloop) {
		return -1;
	}
	_server = new LiveServer(_serverBaseloop, serveraddr, servername, thnum);
	if(!_server) {
		return -1;
	}
	_server->start();
	return 0;
}

void LiveService::dispose()
{
	if(_server) {
		delete _server;
		_server = NULL;
	}
}


