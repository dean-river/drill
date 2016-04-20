#ifndef DRILL_MONITOR_SYSTEM_H_
#define DRILL_MONITOR_SYSTEM_H_
#include <drill/monitor/monitor.h>

using namespace drill::http;


namespace drill {
namespace monitor{

class System
{
 public:
  void registerCommands(Monitor *ins);

  static string overview(HttpMethod, const Monitor::ArgList&);
  static string loadavg(HttpMethod, const Monitor::ArgList&);
  static string version(HttpMethod, const Monitor::ArgList&);
  static string cpuinfo(HttpMethod, const Monitor::ArgList&);
  static string meminfo(HttpMethod, const Monitor::ArgList&);
  static string stat(HttpMethod, const Monitor::ArgList&);
};


}

}
#endif

