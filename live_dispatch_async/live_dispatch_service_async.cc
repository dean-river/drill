#include <live_dispatch_async/live_dispatch_service_async.h>
#include <drill/net/addr_inet.h>
#include <cstdlib>

using namespace drill::net;

LiveServiceAsync::LiveServiceAsync(): _server(NULL)
{

}
LiveServiceAsync::~LiveServiceAsync()
{

}
int LiveServiceAsync::run()
{
	int thnum = _conf.getValue("thread_num",1);
	std::string servername = _conf.getValue("service_name","live_dispath");
	std::string addr = _conf.getValue("server_listen_addr","live_dispath");
	AddrInet serveraddr(addr.c_str());
	_serverBaseloop = _th.startLoop();
	if(!_serverBaseloop) {
		return -1;
	}
	_server = new LiveServerAsync(_serverBaseloop, serveraddr, servername, thnum);
	if(!_server) {
		return -1;
	}
	_server->start();
	return 0;
}

void LiveServiceAsync::dispose()
{
	if(_server) {
		delete _server;
		_server = NULL;
	}
}


