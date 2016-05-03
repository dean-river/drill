#include <drill/service/service.h>
#include <live_dispatch_async/live_dispatch_server_async.h>
#include <drill/event/event_loop_thread.h>

using namespace drill::service;
using namespace drill::event;
class LiveServiceAsync: public Service {
public:
	LiveServiceAsync();
	virtual ~LiveServiceAsync();

private:
	virtual int run();

	virtual void dispose();
	
private:
	LiveServerAsync   *_server;
	EventLoopThread    _th;
	EventLoop        *_serverBaseloop;
	


};

