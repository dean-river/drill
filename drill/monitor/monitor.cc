#include <drill/monitor/monitor.h>
#include <cassert>
#include <functional>

namespace drill {
namespace monitor {

Monitor::Monitor(drill::event::EventLoop *loop):_loop(loop)
{

}
bool Monitor::init()
{
	assert(_loop);
	_cpuInfo = 	std::shared_ptr<CpuInfo>(new CpuInfo());
	_cpu = std::shared_ptr<Cpu>(new Cpu());
	_swap = std::shared_ptr<Swap>(new Swap());
	_memory = std::shared_ptr<Memory>(new Memory);
	_loadAvg = std::shared_ptr<LoadAvg>(new LoadAvg());
	_uptime = std::shared_ptr<Uptime>(new Uptime());
	_procTime = std::shared_ptr<ProcTime>(new ProcTime());
	_procStat = std::shared_ptr<ProcStat>(new ProcStat);
	_procMem = std::shared_ptr<ProcMem>(new ProcMem());
	_procCred = std::shared_ptr<ProcCred>(new ProcCred());
	_procState = std::shared_ptr<ProcState>(new ProcState());
	_procExe = std::shared_ptr<ProcExe>(new ProcExe());
	_threadTime = std::shared_ptr<ThreadTime>(new ThreadTime());

	_loop->runEvery(1*1000*1000,
				std::bind(&Monitor::onTimer,this));

	return true;
	
}
void onTimer()
{

}
}
}

