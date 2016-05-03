#include <live_dispatch/live_dispatch_service.h>
#include <drill/service/service.h>
using namespace drill::service;
int main(int argc, char** argv)
{
	Service *app = new LiveService();

	app->main(argc, argv);
	delete app;
	return 0;

}
