#ifndef DRILL_MONITOR_MONITOR_INFO_H_
#define DRILL_MONITOR_MONITOR_INFO_H_

#include <cstdint>
#include <string>

namespace drill {
namespace monitor {
	
/* /proc/cpuinfo*/
class CpuInfo {
public:
	CpuInfo();
	bool upDate();
public:
  std::string  vender;
  std::string  model;
  int64_t cache_size;
  int32_t mhz;
  int32_t total_socket;
  int32_t core_per_socket;
  
};
/*
  /proc/stat the first line total stat
*/

class Cpu {
public:
	Cpu()
		:user(0),
		 sys(0),
		 nice(0),
		 idle(0),
		 wait(0),
		 irq(0),
		 soft_irq(0),
		 stolen(0),
		 total(0)
	{}

bool upDate();

public:
  int64_t user;
  int64_t sys;
  int64_t nice;
  int64_t idle;
  int64_t wait;
  int64_t irq;
  int64_t soft_irq;
  int64_t stolen;
  int64_t total;
  
};
/*
* /proc/meminfo
*/

class Swap {

public:
  int64_t total;
  int64_t used;
  int64_t free;
  int64_t page_in;
  int64_t page_out;
};

class Memory {
public:

public:
  int64_t ram;
  int64_t total;
  int64_t used;
  int64_t free;
  int64_t actual_used;
  int64_t actual_free;
  double  used_percent;
  double  free_percent;
};

class LoadAvg {
public:
  double loadavg[3];
};

/* /proc/uptime */
class Uptime{
public:
  Uptime(double t = 0):uptime(t) {}

  bool update();
public:
  double uptime;
};

class ProcTime {

public:
  int64_t start_time;
  int64_t user;
  int64_t sys;
  int64_t total;
};

class ProcStat{

public:
  int64_t total;
  int64_t sleeping;
  int64_t running;
  int64_t zombie;
  int64_t stoped;
  int64_t idle;
  int64_t threads;
};

class ProcMem {
public:
	int64_t size;
	int64_t resident;
	int64_t share;
	int64_t minor_faults;
	int64_t major_faults;
	int64_t page_faults;
	

};

class ProcCred {
public:
	int32_t pid;
	int32_t gid;
	int32_t uid;
	int32_t euid;
	int32_t egid;
};

class ProcState {
public:
	char    name[256];
	char    state;
	int32_t ppid;
	int32_t tty;
	int32_t priority;
	int32_t nice;
	int32_t processor;
	int32_t threads;
	int32_t fds;
};

class ProcExe{
public:
	char name[4096];
	char cwd[4096];
	char root[4096];
	
};

class ThreadTime{
public:
	int64_t user;
	int64_t sys;
	int64_t total;
};

/* disk*/

class FilesystemUsage {
public:
	
	
	double use_percent;
	uint64_t total;
	uint64_t free;
	uint64_t used;
	uint64_t avail;
	uint64_t files;
	uint64_t free_files;
		
};



}
}
#endif
