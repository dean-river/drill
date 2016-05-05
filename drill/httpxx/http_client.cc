#include <drill/httpxx/http_client.h>


using namespace drill::net;
using namespace drill::event;
using namespace drill::httpxx;


namespace drill {
namespace httpxx {

HttpClient::HttpClient(EventLoop * loop,AddrInet & serverAddr, string & name)
	:_serverAddr(serverAddr),_loop(loop),_client(loop,serverAddr,name)
{
	_conn = _client.connection();
	_client.setConnectionCallback(std::bind(&HttpClient::onConnection, this, std::placeholders::_1));
	_client.setMessageCallback(std::bind(&HttpClient::onMessage, this, 
		std::placeholders::_1,  std::placeholders::_2,  std::placeholders::_3));
	_client.setWriteCompleteCallback(std::bind(&HttpClient::onWriteComplete, this, std::placeholders::_1));
}

void HttpClient::connect()
{
	_client.connect();
}
void HttpClient::shutdown()
{
	_client.disconnect();
}

void HttpClient::onMessage(const TcpConnectionPtr &conn,
						  Buffer *buf,
						  Time recvtime)
{
	_lastrecv = recvtime;
	while (buf->readableBytes()) {
		if(!_currentResponse) {
			_currentResponse = _responseFactory();
		}
		if(!_currentResponse) {
			conn->shutdown();
		}
	
		size_t used = _currentResponse->feed(buf->peek(), buf->readableBytes());
		buf->retrieve(used);
		if(!_currentResponse->isValid()) {
			if(_resParseError) {
				_resParseError(this);
			}
			conn->shutdown();
			return ;
		}
		if(_currentResponse->complete()) {
			_resReady(_currentResponse, this);
			_currentResponse.reset();
		}
	}
	
}
void HttpClient::onConnection(const TcpConnectionPtr &conn)
{
	if(conn->connected()) {
		_conn = conn;
		if(_onConnect) {
			_onConnect(this);
		}
	} else {
		if(_onClose) {
			_onClose(this);
		}
	}
}
bool HttpClient::isSendComplete()
{
	if(!_currentRequest) {
		return true;
	}
	if(!_currentRequest->isSendComplete()) {
		return false;
	}
	return true;
}

void HttpClient::onWriteComplete(const TcpConnectionPtr &conn)
{
	(void)conn;
	if(!isSendComplete()) {
		sendReques();
		return ;
	}

	_currentRequest.reset();
	if(_writeComplete) {
		_writeComplete(this);
	}
}

bool HttpClient::postRequest(RequestBuilderPtr &req)
{
	if(_currentRequest) {
		return false;
	}

	_currentRequest = req;
	sendReques();
	return true;
	
}
void HttpClient::sendReques()
{
	if(_loop->isInLoopThread()) {
		sendRequestInloop();
	} else {
		_loop->runInLoop(std::bind(&HttpClient::sendRequestInloop, this));
	}
}
void HttpClient::sendRequestInloop()
{
	string msg;
	_reqWrite(_currentRequest, msg);
	TcpConnectionPtr c = _conn.lock();
	c->send(msg);
}

void HttpClient::setResponseReady(const ResponseReady &cb)
{
	_resReady = cb;
}

void HttpClient::setContext(const any &any)
{
	_context = any;
}
const any HttpClient::getContext()
{
	return _context;
}
any *HttpClient::getMutableContext()
{
	return &_context;
}

void HttpClient::setRequestWrite(const RequestWrite &cb)
{
	_reqWrite = cb;
}
void HttpClient::setResponseFactory(const ResponseFactory &cb)
{
	_responseFactory = cb;
}
HttpClient::~HttpClient()
{

}

}
}
