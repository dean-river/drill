#include <drill/monitor/monitor.h>
#include <drill/common/scoped_lock.h>
#include <drill/common/logger.h>

#include <cassert>
#include <functional>


using namespace drill::http;
using namespace drill::net;
using namespace drill::common;


namespace drill {
namespace monitor {

Monitor::Monitor(drill::event::EventLoop *loop, 
     AddrInet &httpaddr, std::string & name):_server(loop, httpaddr, name)
{

}

bool Monitor::init()
{
	sigar_open(&g_ar);
	if(!g_ar) {
		return false;
	}
	
	return true;
	
}

void Monitor::add(const string& module,			 const string& command, 		  
			const Callback& cb, 		  const string& help)
{
	ScopedMutex lock(&_mutex);
	 _modules[module][command] = cb;
	 _helps[module][command] = help;
	
}

void Monitor:: remove(const string& module, const string& command)
{
	ScopedMutex lock(&_mutex);
	std::map<string, CommandList>::iterator it = _modules.find(module);
	if (it != _modules.end())  {
		it->second.erase(command);    
		_helps[module].erase(command);
	}
}

bool Monitor::start()
{
	
	_server.start();
	return true;
}
void Monitor::onRequest(const HttpRequest& req, HttpResponse* resp)
{
	if (req.path() == "/") {
	   string result;
	   ScopedMutex lock(&_mutex);
	   for (std::map<string, HelpList>::const_iterator helpListI = _helps.begin();
			helpListI != _helps.end();
			++helpListI) {
				
		 const HelpList& list = helpListI->second;
		 for (HelpList::const_iterator it = list.begin();
			  it != list.end();
			  ++it) {
		   result += "/";
		   result += helpListI->first;
		   result += "/";
		   result += it->first;
		   size_t len = helpListI->first.size() + it->first.size();
		   result += string(len >= 25 ? 1 : 25 - len, ' ');
		   result += it->second;
		   result += "\n";
		 }
	   }
	   resp->setStatusCode(ok);
	   resp->setStatusMessage("OK");
	   resp->setContentType("text/plain");
	   resp->setBody(result);
	 } else {
	   std::vector<string> result;
	   string_split(req.path(), '/', result);
	   
	   bool ret = false;
	   if (result.size() == 0) {
		 LOG_DEBUG << req.path();
	   } else if (result.size() == 1) {
		 LOG_ERROR << "Unimplemented " << result[1];
	   } else {
		 string module = result[0];
		 std::map<string, CommandList>::const_iterator commListI = _modules.find(module);
		 if (commListI != _modules.end()) {
		   string command = result[1];
		   const CommandList& commList = commListI->second;
		   CommandList::const_iterator it = commList.find(command);
		   if (it != commList.end()) {
			 ArgList args(result.begin()+2, result.end());
			 if (it->second) {
			   resp->setStatusCode(drill::http::ok);
			   resp->setStatusMessage("OK");
			   resp->setContentType("text/plain");
			   const Callback& cb = it->second;
			   resp->setBody(cb(req.method(), args));
			   ret= true;
			 }
		   }
		 }
	
	   }
	
	   if (!ret)
	   {
		 resp->setStatusCode(drill::http::not_found);
		 resp->setStatusMessage("Not Found");
	   }
	 }


}
}
}

