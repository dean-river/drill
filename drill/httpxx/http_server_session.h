#ifndef DRILL_HTTPXX_HTTP_SESSION_H_
#define DRILL_HTTPXX_HTTP_SESSION_H_
#include <drill/common/any.h>
#include <drill/common/buffer.h>
#include <drill/common/time.h>
#include <drill/common/blocked_queue.h>
#include <drill/httpxx/http_message_factory.h>
#include <drill/httpxx/http_request.h>
#include <drill/httpxx/http_response_builder.h>
#include <drill/net/tcp_connection.h>
#include <string>
#include <list>
#include <memory>
using drill::common::any;
using drill::common::Time;
using drill::common::Buffer;
using drill::common::BlockQueue;
using namespace drill::net;

namespace drill {
namespace httpxx {

class HttpServerSession;
typedef std::function<bool(RequestPtr, HttpServerSession*)>  RequestReady;
typedef std::function<bool(ResponseBuilderPtr, RequestPtr)>         ResponseWriteComplete;
typedef std::function<bool(HttpServerSession*)>              SessionCB;

class HttpServerSession {
public:
	HttpServerSession(const TcpConnectionPtr &con);
	virtual ~HttpServerSession();

	void setContext(any & c);

	void setMessageFactory(MessageFactor *factor);

	MessageFactor *getMessageFactory();
	
	bool parseMessage(Buffer *buf, Time &recvtime);

	bool gotCurrentMessage();

	void postResponse(const ResponseBuilderPtr & res);

	void sendMessage();

	void onSendComplete();

	bool closeConnection();

	

	void setClose(bool ifclose);
	
	void setRequestReadyCB(const RequestReady &cb);
	void setResponseWriteComplete(const ResponseWriteComplete &cb);
	void setOnParseError(const SessionCB &cb);
private:
	void sendMessageInloop();
	bool isSendComplete();
	void processBadRequest();
private:
	RequestReady                   _reqReady;
	ResponseWriteComplete          _resWriteComplete;
	SessionCB                      _onParseError;
private:
	std::weak_ptr<TcpConnection>    _conn;
	MessageFactor                  *_factory;
	bool                            _close;
	Time                            _lastRecv;
	any                             _context;
	RequestPtr                      _currentRequest;
	ResponseBuilderPtr              _currentResponse;
	BlockQueue<RequestPtr>          _reqQueue;
	BlockQueue<ResponseBuilderPtr>  _resQueue;
};

typedef std::shared_ptr<HttpServerSession> HttpServerSessionPtr;
}
}
#endif