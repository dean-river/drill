#include <drill/monitor/monitor_info.h>
#include <drill/common/filesystem.h>
#include <drill/common/properties.h>
#include <drill/common/string.h>
#include <cstring>
#include <string>
#include <cstdlib>

using namespace drill::common;
using namespace std;

namespace drill {
namespace monitor {
CpuInfo::CpuInfo()
{
    cache_size = 0;
  	mhz = 0;
  	total_socket = 0;
    core_per_socket = 0;
}
bool CpuInfo::upDate()
{
	Properties info;
	if (!info.loadProperties("/proc/cpuinfo" , ':', false)) {
		return false;
	}

	vender = info.getValue("vendor","");
	mhz = info.getValue("cpu MHz",0);
	string c = info.getValue("cache size","");
	uint64_t ret;
	parse_size(c.c_str(), ret);
	cache_size = ret;
	return true;
		
}
	

bool Cpu::upDate()
{
	return true;
}


}

}
