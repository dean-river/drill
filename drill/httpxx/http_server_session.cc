#include <drill/httpxx/http_server_session.h>
#include <drill/event/event_loop.h>
#include <drill/common/logger.h>

using namespace drill::event;
using namespace drill::net;

namespace drill {
namespace httpxx {

HttpServerSession::HttpServerSession(const TcpConnectionPtr &con)
{
	_conn = con;
	_factory = NULL;
	_close = false;
}

HttpServerSession::~HttpServerSession()
{

}

void HttpServerSession::setContext(any & c)
{
	_context = c;
}

bool HttpServerSession::parseMessage(Buffer *buf, Time &recvtime)
{
	_lastRecv = recvtime;
	size_t used = 0;
	while(buf->readableBytes()) {

		if(!_currentRequest) {
			_currentRequest = _factory();
			if(!_currentRequest) {
			LOG_ERROR<<"create request error";
			return false;
			}
		}

		used = _currentRequest->feed(buf->peek(), buf->readableBytes());
		buf->retrieve(used);
		if(!_currentRequest->isValid()) {
			processBadRequest();
			return false;
		}
		if(gotCurrentMessage()) {
			_reqReady(_currentRequest, this);
			_reqQueue.put(_currentRequest);
			_currentRequest.reset();
		}
	}
		return true;
}

void HttpServerSession::processBadRequest()
{
	static std::string badReq("HTTP/1.1 400 Bad Request\r\n\r\n");
	if(_onParseError) {
		_onParseError(this);
	}
	TcpConnectionPtr cn = _conn.lock();
	if(cn) {
		cn->send(badReq);
		_close = true;
		cn->shutdown();
	}
}

bool HttpServerSession::gotCurrentMessage()
{
	return _currentRequest->complete();
}

void HttpServerSession::postResponse(const ResponseBuilderPtr & res)
{
	_resQueue.put(res);
	sendMessage();
}

bool HttpServerSession::isSendComplete()
{
	if(!_currentResponse->isSendComplete()) {
		return false;
	}
	if(_resQueue.size()) {
		return false;
	}
	return true;
}

void HttpServerSession::sendMessageInloop()
{
	if(!_currentResponse) {
		if(_resQueue.size()) {
			_currentResponse = _resQueue.take();
		}
	}

	std::string mes;
	_resWrite(_currentResponse, mes);
	TcpConnectionPtr cn = _conn.lock();
	if(cn) {
		cn->send(mes);
	}
	
}
void HttpServerSession::sendMessage()
{
	TcpConnectionPtr cn = _conn.lock();	
	if(!cn) {
		return;
	}
	EventLoop *loop = cn->getLoop();
	if(loop->isInLoopThread()) {
		sendMessageInloop();
	} else {
		loop->runInLoop(std::bind(&HttpServerSession::sendMessageInloop,this));
	}
}

void HttpServerSession::onSendComplete()
{
	if(!isSendComplete()) {
			sendMessage();
			return;
	}
	if(_currentResponse) {
		RequestPtr req = _reqQueue.take();
		if(_currentResponse->isSendComplete()) {
			if(_resWriteComplete) {
				_resWriteComplete(_currentResponse, req);
				_close = _currentResponse->ifClose();
				_currentResponse.reset();	
			}	
		}
		
		if(_close) {
			TcpConnectionPtr cn = _conn.lock();
			if(cn) {
				cn->shutdown();
			}
			return ;
		}
	}

		
}
void HttpServerSession::setResponseWrite(const ResponseWrite &cb)
{
	_resWrite = cb;
}

void HttpServerSession::setRequestReadyCB(const RequestReady &cb)
{
	_reqReady = cb;
}
void HttpServerSession::setResponseWriteComplete(const ResponseWriteComplete &cb)
{
	_resWriteComplete = cb;
}

void HttpServerSession::setOnParseError(const SessionCB &cb)
{
	_onParseError = cb;
}

void HttpServerSession::setRequestCreate(const RequestFactory &factor)
{
	_factory = factor;
}

bool HttpServerSession::closeConnection()
{
	return _close;
}

}
}
