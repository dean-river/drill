#include <live_dispatch_async/live_dispatch_server_async.h>
#include <drill/httpxx/http_server_session.h>
#include <drill/httpxx/http_request.h>
#include <drill/httpxx/http_response_builder.h>
#include <drill/httpxx/http_buffered_message.h>
#include <drill/httpxx/http_url.h>

#include <drill/common/string.h>
#include <drill/common/logger.h>

#include <functional>
#include <memory>
#include <iostream>
using namespace drill::net;
using namespace drill::common;
using namespace drill::event;
using namespace drill::httpxx;
using rapidjson::Document;
using rapidjson::Value;

namespace detail {

	class LiveResponseBuilder: public ResponseBuilder {
	public:
		LiveResponseBuilder()
		{
			_senddone = false;
		}

		bool setUp()
		{
			Message::Headers& h = headers();
			_body = "hello world!\n";
			h["Content-Length"] = to_str(_body.size());
			h["Connection"] = "Keep-Alive";
			h["Content-Type"] = "text/plain";
			h["Server"] = "live_server_async";
			setStatus(200);
			setMinorVersion(1);
			return true;
		}

		bool setRes404()
		{
			Message::Headers& h = headers();
			_body = "bad path!\n";
			h["Content-Length"] = to_str(_body.size());
			h["Connection"] = "Keep-Alive";
			h["Content-Type"] = "text/plain";
			h["Server"] = "live_server_async";
			setStatus(404);
			setMinorVersion(1);
			return true;
		}

		bool isSendComplete() { return _senddone; }
		
		virtual bool sendMessage( std::string & mes)
		{
			
			mes += statusLine();
			mes += headersToString();
			mes += "\r\n";
			mes += _body;
			_senddone = true;
			LOG_ERROR<<"response: "<< mes;
			return true;
		}
	private:
		std::string _body;
		bool        _senddone;
	};

	
	bool writeResponse(ResponseBuilderPtr &res, std::string &ms)
	{
		LiveResponseBuilder  *r = (LiveResponseBuilder*)res.get();
		r->sendMessage(ms);
		return true;
	}
	RequestPtr createRequest()
	{
		std::shared_ptr<BufferedRequest> ret(new BufferedRequest);
		return ret;
	}
	bool onRequestReady(RequestPtr req, HttpServerSession *hss)
	{
		BufferedRequest *r = (BufferedRequest*)(req.get());
		LiveResponseBuilder *lres = new LiveResponseBuilder();
		ResponseBuilderPtr res(lres);
		Url u(r->url());
		if(u.path() != "/live/addr") {
			lres->setRes404();
			hss->postResponse(res);
			return false;
		}
		lres->setUp();
		hss->postResponse(res);
		return true;
	}

	bool onResponseWriteComplete(ResponseBuilderPtr& res, RequestPtr & req)
	{
		(void)req;
		res->setClose(false);
		return true;
	}
}
LiveServerAsync::LiveServerAsync(EventLoop *loop, AddrInet& listenAddr, const std::string& name, int threadnum)
:_server(loop, listenAddr, name)
{
	_server.setThreadNum(threadnum);
	_server.setSessionBuilder(std::bind(& LiveServerAsync::builder, this, std::placeholders::_1));
	_server.setSessionClear(std::bind(& LiveServerAsync::clear, this, std::placeholders::_1));
}

bool LiveServerAsync::builder(HttpServerSession *hss)
{
	hss->setResponseWrite(std::bind(detail::writeResponse, std::placeholders::_1, std::placeholders::_2));
	hss->setRequestCreate(std::bind(detail::createRequest));
	hss->setRequestReadyCB(std::bind(detail::onRequestReady, std::placeholders::_1, std::placeholders::_2));
	hss->setResponseWriteComplete(std::bind(detail::onResponseWriteComplete, std::placeholders::_1, std::placeholders::_2));
	return true;
}
bool LiveServerAsync::clear(HttpServerSession *hss)
{
	(void)hss;
	return true;
}


