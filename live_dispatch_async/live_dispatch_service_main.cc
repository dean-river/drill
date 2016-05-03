#include <live_dispatch_async/live_dispatch_service_async.h>
#include <drill/service/service.h>
using namespace drill::service;
int main(int argc, char** argv)
{
	Service *app = new LiveServiceAsync();

	app->main(argc, argv);
	delete app;
	return 0;

}
