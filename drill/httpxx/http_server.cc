#include <drill/httpxx/http_server.h>
#include <drill/httpxx/http_session.h>

using namespace drill::net;
using namespace drill::common;
using namespace drill::event;


namespace drill {
namespace httpxx {

HttpServer::HttpServer(EventLoop* loop,
		   AddrInet& listenAddr,
		   const string& name,
		   TcpServer::Option option)
		   :  : _server(loop, listenAddr, name, option),
{
	_server.setConnectionCallback(
      std::bind(&HttpServer::onConnection, this, std::placeholders::_1));
	_server.setMessageCallback(
      std::bind(&HttpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
	_server.setWriteCompleteCallback(
      std::bind(&HttpServer::onWriteComplete, this, std::placeholders::_1));
}
void HttpServer::onConnection(const TcpConnectionPtr& conn)
{
	if(conn->connected()) {
		assert(_builder);
		HttpSession *session = new HttpSession();
		if(!_builder(session)) {
			conn->shutdown();
		}
		string addr = conn->peerAddress().toString();
		conn->setContext(session);
	} else {
		any *mc = conn->getMutableContext();
		HttpSession *s = static_cast<HttpSession*>(mc);
		delete s;
	}
	
}
void HttpServer::onWriteComplete(const TcpConnectionPtr& conn)
{
	any *mc = conn->getMutableContext();
	HttpSession *s = static_cast<HttpSession*>(mc);
	if(s->closeConnection()) {
		delete s;
		conn->shutdown();
	}
}

void HttpServer::onMessage(const TcpConnectionPtr& conn,
				Buffer* buf, Time receiveTime)
{
	any *mc = conn->getMutableContext();
	HttpSession *s = static_cast<HttpSession*>(mc);

	while(buf->readableBytes()) {
		if(!s->parseRequest(buf,receiveTime)) {
			conn.send(s->processBadRequest());
			conn->shutdown();
			return;
		}

		if(s->getCurrentRequest()) {
			s->handleRequest();
		}
	}
}

}
}
