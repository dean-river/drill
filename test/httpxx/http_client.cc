#include <drill/httpxx/http_client.h>
#include <drill/httpxx/http_method.h>

#include <drill/httpxx/http_request_builder.h>
#include <drill/httpxx/http_buffered_message.h>
#include <iostream>
#include <drill/common/string.h>

using namespace drill::httpxx;
using namespace drill::common;
using namespace drill::event;
using namespace drill::net;

int cnt;
EventLoop *loop;
class TestRequestBuidler:public RequestBuilder {
public:
	TestRequestBuidler() {}
	void setBody(std::string b) {
		_body = b;
	}

	void setUp()
	{
		setUrl("/live/addr?uid=106839&proto=5&cv=Iphone5s");
		_body = "1111111111";
				setMethod(Method::post());
		setMinorVersion(1);
		Message::Headers &h = headers();
		h["Content-Length"] = to_str(_body.size());
		h["Connection"] = "Keep-Alive";
		h["Content-Type"] = "text/plain";
		h["host"] = "127.0.0.1";
	}

	void setUpbad()
	{
		setUrl("/live/ddr?uid=106839&proto=5&cv=Iphone5s");
		setMethod(Method::post());
		setMinorVersion(1);
		_body = "1111111111";
		Message::Headers &h = headers();
		h["Content-Length"] = to_str(_body.size());
		h["Connection"] = "Keep-Alive";
		h["Content-Type"] = "text/plain";
		h["host"] = "127.0.0.1";
	}
	virtual bool sendMessage( string & mes)
	{
		mes = requestLine();
		mes += headersToString();
		mes += "\r\n";
		mes += _body;
		return true;
	}
private:
	std::string _body;

};

namespace detail {
bool onResponse(ResponsePtr& res, HttpClient*hc) 
{
	cnt++;
	if(cnt >2) {
		hc->shutdown();
		return true;
	}
	TestRequestBuidler *req = new TestRequestBuidler();
	BufferedResponse *br = (BufferedResponse *)res.get();
	std::cout<<br->status()<<"\r\n"<<br->body()<<"\n";
	RequestBuilderPtr s(req);
	req->setUpbad();
	hc->postRequest(s);
	return true;
}

bool onConnect(HttpClient* hc)
{
	cnt = 0;
	TestRequestBuidler *req = new TestRequestBuidler();
	RequestBuilderPtr s(req);
	req->setUp();
	hc->postRequest(s);
	return true;
}

bool onClose(HttpClient* hc)
{
	(void)hc;
	loop->quit();
	return true;
}

bool onRequestWrite(RequestBuilderPtr& req, std::string& ms)
{
	TestRequestBuidler *r = (TestRequestBuidler *)req.get();

	r->sendMessage(ms);
		std::cout<<ms<<std::endl;
		return true;
}

ResponsePtr factory()
{
	BufferedResponse *res = new BufferedResponse();
	return ResponsePtr(res);
}
}
int main()
{
	EventLoop l;
	loop = &l;
	AddrInet s("127.0.0.1",8088);
	std::string serverName("test");
	HttpClient hc(&l, s, serverName);
	hc.setCloseCB(std::bind(detail::onClose,std::placeholders::_1));
	hc.setRequestWrite(std::bind(detail::onRequestWrite, std::placeholders::_1, std::placeholders::_2));
	hc.setResponseReady(std::bind(detail::onResponse, std::placeholders::_1, std::placeholders::_2));
	hc.setResponseFactory(std::bind(detail::factory));
	hc.setConnectCB(std::bind(detail::onConnect,std::placeholders::_1));
	hc.connect();
	l.loop();
	return 0;
}

