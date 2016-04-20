#ifndef DRILL_MONITOR_MONITOR_H_
#define DRILL_MONITOR_MONITOR_H_
#include <drill/http/http_server.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>
#include <drill/common/mutex.h>
#include <drill/monitor/sigar.h>
#include <memory>
#include <map>
#include <string>
#include <functional>

using namespace drill::http;
using namespace drill::net;
using namespace drill::common;

namespace drill {

namespace monitor {

sigar_t                      *g_ar;

class Monitor {
public:
  typedef std::vector<string> ArgList;
  typedef std::function<string (HttpMethod, const ArgList& args)> Callback;
	Monitor(drill::event::EventLoop *loop, 
     AddrInet &httpaddr, std::string & name);

  void add(const string& module,           const string& command,           
              const Callback& cb,           const string& help);
            
  void remove(const string& module, const string& command);

  bool init();
  
  bool start();
private:
  typedef std::map<string, Callback> CommandList;
  typedef std::map<string, string> HelpList;
  
  HttpServer _server;
  Mutex      _mutex;
  std::map<string, CommandList> _modules;
  std::map<string, HelpList>    _helps;

private:
	void onRequest(const HttpRequest& req, HttpResponse* resp);
	
};
}
}
#endif
