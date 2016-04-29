#include <drill/http/http_server.h>
#include <drill/http/http_request.h>
#include <drill/http/http_response.h>
#include <drill/common/logger.h>
#include <drill/event/event_loop.h>
#include <drill/net/addr_inet.h>
#include <drill/ext/json/document.h>
#include <drill/common/any.h>

#include <iostream>
#include <map>

using namespace drill::common;
using namespace drill::event;
using namespace drill::http;
using namespace drill::net;

using rapidjson::Document;
using rapidjson::Value;

bool benchmark = false;

void onRequest(const HttpRequest& req, HttpResponse* resp)
{

  if (req.path() == "/")
  {
  	//std::cout<<req.toString()<<std::endl;
    resp->setStatusCode(ok);
    resp->setStatusMessage("OK");
    resp->setContentType("text/html");
    resp->addHeader("Server", "live_dispatch");
    string now = Time::now().toDateTime();
	string b ="<html><head><title>This is title</title></head>"
        "<body><h1>Hello</h1>Now is " + now +string(1024*5,'a')+
        "</body></html>";
    resp->setBody(b);
  }
  else if (req.path() == "/live/addr")
  {
  
  	std::cout<<"uid: "<<req.getArg(string("uid"))<<endl;
	std::cout<<"proto: "<<req.getArg("proto")<<endl;
	std::cout<<"cv: "<<req.getArg("cv")<<endl;
  	std::cout<<"body: "<<req.getbody()<<endl;

	Document doc;
    doc.Parse<0>(req.getbody().c_str());

	if(doc.HasParseError()) {
		 resp->setStatusCode(ok);
   		 resp->setStatusMessage("OK");
   		 resp->setContentType("text/plain");
    	 resp->addHeader("Server", "live_dispatch");
    	 resp->setBody("bad body!\n");
		 return ;
	}

    Value & v = doc["id"];
	string id = v.GetString();
    resp->setStatusCode(ok);
	std::cout<<"id: "<<id<<endl;
    resp->setStatusMessage("OK");
    resp->setContentType("text/plain");
    resp->addHeader("Server", "live_dispatch");
    resp->setBody("hello, world!\n");
	
  } else{
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

