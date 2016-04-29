#include <drill/httpxx/http_session.h>

namespace drill {
namespace httpxx {

void HttpSession::setContext(any & c)
{
	_context = c;
}

bool HttpSession::parseRequest(Buffer *buf, Time &recvtime)
{
	_lastRecv = recvtime;
	if(!_currentRequest) {
		_currentRequest = new BufferedRequest();
	}
	size_t used = _currentRequest->feed(buf->peek(), buf->readableBytes());
	buf->retrieve(used);
	if(!_currentRequest->isvalid()) {
		return false;
	}
	
	return true;

}

std::string HttpSession::processBadRequest()
{
	static std::string badReq("HTTP/1.1 400 Bad Request\r\n\r\n");
	return badReq;
}

bool HttpSession::getCurrentRequest()
{
	return _currentRequest->complete();
}

bool HttpSession::handleRequest()
{
	if(!getCurrentRequest()) {
		return true;
	}
	
	_requestList.push_back(_currentRequest);
	_currentRequest = new BufferedRequest();
	
}

bool HttpSession::closeConnection()
{
	return _close;
}

}
}
