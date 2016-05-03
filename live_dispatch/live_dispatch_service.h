#include <drill/service/service.h>
#include <live_dispatch/live_dispatch_server.h>
#include <drill/event/event_loop_thread.h>

using namespace drill::service;
using namespace drill::event;
class LiveService: public Service {
public:
	LiveService();
	virtual ~LiveService();

private:
	virtual int run();

	virtual void dispose();
	
private:
	LiveServer       *_server;
	EventLoopThread   _th;
	EventLoop        *_serverBaseloop;
	


};

