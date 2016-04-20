#include <drill/monitor/system.h>
#include <drill/common/filesystem.h>

using namespace drill::monitor;
using namespace drill::http;
using namespace drill::common;

void System::registerCommands(Monitor *ins)
{
  ins->add("sys", "overview", System::overview, "print system overview");
  ins->add("sys", "loadavg", System::loadavg, "print /proc/loadavg");
  ins->add("sys", "version", System::version, "print /proc/version");
  ins->add("sys", "cpuinfo", System::cpuinfo, "print /proc/cpuinfo");
  ins->add("sys", "meminfo", System::meminfo, "print /proc/meminfo");
  ins->add("sys", "stat", System::stat, "print /proc/stat");
}

string System::loadavg(HttpMethod, const Monitor::ArgList&)
{
  string loadavg;
  readFileToString("/proc/loadavg", &loadavg);
  return loadavg;
}

string System::version(HttpMethod, const Monitor::ArgList&)
{
  string version;
  readFileToString("/proc/version", &version);
  return version;
}

string System::cpuinfo(HttpMethod, const Monitor::ArgList&)
{
  string cpuinfo;
  readFileToString("/proc/cpuinfo", &cpuinfo);
  return cpuinfo;
}

string System::meminfo(HttpMethod, const Monitor::ArgList&)
{
  string meminfo;
  readFileToString("/proc/meminfo", &meminfo);
  return meminfo;
}

string System::stat(HttpMethod, const Monitor::ArgList&)
{
  string stat;
  readFileToString("/proc/stat", &stat);
  return stat;
}

string System::overview(HttpMethod, const Monitor::ArgList&)
{
  string result("overview");
  return result;
}

