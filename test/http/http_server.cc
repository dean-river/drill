#include <drill/http/http_server.h>
#include <drill/http/http_request.h>
#include <drill/http/http_response.h>
#include <drill/common/logger.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>

#include <iostream>
#include <map>

using namespace drill::common;
using namespace drill::event;
using namespace drill::http;
using namespace drill::net;

extern char favicon[555];
bool benchmark = false;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  //std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<string, string>& headers = req.args();
    for (std::map<string, string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
      std::cout << it->first << "= " << it->second << std::endl;
    }
  }

  if (req.path() == "/")
  {
  	//std::cout<<req.toString()<<std::endl;
    resp->setStatusCode(ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "walle");
    string now = Time::now().toDateTime();
	string b ="<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +string(1024*5,'a')+
        "</body></html>";
    resp->setBody(b);
  }
  else if (req.path() == "/hello")
  {
    resp->setStatusCode(ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "walle");
    resp->setBody("hello, world!\n");
  } else if(req.path() == "/create_seed") {
  /*
		todo queue to a task list;
  */
	resp->setStatusCode(ok);
    resp->setStatusMessage("OK");
  }else{
    resp->setStatusCode(not_found);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
}

int main(int argc, char* argv[])
{
  int numThreads = 0;
  if (argc > 1)
  {
    benchmark = true;
    numThreads = atoi(argv[1]);
	Logger::setLogLevel(Logger::WARN);
  }
  EventLoop loop;
  AddrInet Addr("0.0.0.0:8080");
  HttpServer server(&loop, Addr, "walle");
  server.setHttpCallback(onRequest);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
}

