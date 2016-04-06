#ifndef WALLE_SYS_PROCESSINFO_H_
#define WALLE_SYS_PROCESSINFO_H_

#include <drill/common/types.h>
#include <drill/common/time.h>
#include <unistd.h>
#include <sys/types.h>

#include <vector>
#include <string>
using namespace std;

namespace drill {

namespace ProcessInfo{
  pid_t pid();
  string pidString();
  uid_t uid();
  string username();
  uid_t euid();
  walle::sys::Time startTime();

  string hostname();
  string procname();

  /// read /proc/self/status
  string procStatus();

  /// read /proc/self/stat
  string procStat();

  /// readlink /proc/self/exe
  string exePath();

  int openedFiles();
  int maxOpenFiles();

  int numThreads();
  std::vector<pid_t> threads();
}

}

#endif
