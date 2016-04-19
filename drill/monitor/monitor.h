#ifndef DRILL_MONITOR_MONITOR_H_
#define DRILL_MONITOR_MONITOR_H_

#include <drill/monitor/monitor_info.h>
#include <drill/event/event_loop.h>
#include <memory>

namespace drill {

namespace monitor {

class Monitor {
public:
	Monitor(drill::event::EventLoop *loop);

	bool init();

	std::shared_ptr<CpuInfo> getCupInfo() { return _cpuInfo; } 

	std::shared_ptr<Cpu>  getCpu() { return _cpu; }

	std::shared_ptr<Swap> getSwap() { return _swap; }

	std::shared_ptr<Memory> getMemory() { return _memory; }

	std::shared_ptr<LoadAvg> getLoadAvg() { return _loadAvg; }

	std::shared_ptr<Uptime> getUpTime() { return _uptime; }

	std::shared_ptr<ProcTime> getProcTime() { return _procTime; }

	std::shared_ptr<ProcStat> getProcStat() { return _procStat; }

	std::shared_ptr<ProcMem> getProcMem() { return _procMem; }

	std::shared_ptr<ProcCred> getProcCred() { return _procCred; }

	std::shared_ptr<ProcState> getProcState() { return _procState; }

	std::shared_ptr<ProcExe> getProcExe() { return _procExe; }

	std::shared_ptr<ThreadTime> getThreadTime() { return _threadTime; }
	
private:
	drill::event::EventLoop    *_loop;
	std::shared_ptr<CpuInfo>    _cpuInfo;
	std::shared_ptr<Cpu>        _cpu;
	std::shared_ptr<Swap>       _swap;
	std::shared_ptr<Memory>     _memory;
	std::shared_ptr<LoadAvg>    _loadAvg;
	std::shared_ptr<Uptime>     _uptime;
	std::shared_ptr<ProcTime>   _procTime;
	std::shared_ptr<ProcStat>   _procStat;
	std::shared_ptr<ProcMem>    _procMem;
	std::shared_ptr<ProcCred>   _procCred;
	std::shared_ptr<ProcState>  _procState;
	std::shared_ptr<ProcExe>    _procExe;
	std::shared_ptr<ThreadTime> _threadTime;
private:
	void onTimer();
	
};
}
}
#endif
