#include <live_dispatch/live_dispatch_server.h>
#include <functional>
#include <iostream>
using namespace drill::net;
using namespace drill::event;
using namespace drill::http;
using rapidjson::Document;
using rapidjson::Value;

LiveServer::LiveServer(EventLoop *loop, AddrInet& listenAddr, const std::string& name, int threadnum)
:_server(loop, listenAddr, name)
{
	_server.setHttpCallback(std::bind(&LiveServer::onRequest, this, std::placeholders::_1,  std::placeholders::_2));
	_server.setThreadNum(threadnum);
}
void LiveServer::onRequest(const HttpRequest& req, HttpResponse* resp)
{
	 if (req.path() == "/") {
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
  
  	LOG_WARN<<"uid: "<<req.getArg(string("uid"))
		<<"proto: "<<req.getArg("proto")
		<<"cv: "<<req.getArg("cv")
  		<<"body: "<<req.getbody();

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
	LOG_WARN<<"id: "<<id;
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

