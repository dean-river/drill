#ifndef DRILL_HTTPXX_HTTP_CLIENT_H_
#define DRILL_HTTPXX_HTTP_CLIENT_H_
#include <drill/net/tcp_client.h>
#include <drill/net/tcp_connection.h>
#include <drill/event/event_loop.h>
#include <drill/httpxx/http_request_builder.h>
#include <drill/httpxx/http_response.h>
#include <drill/common/blocked_queue.h>
#include <drill/common/any.h>
#include <memory>
#include <functional>

using namespace drill::net;
using namespace drill::event;
using namespace drill::httpxx;
using namespace drill::common;
namespace drill {
namespace httpxx {

class HttpClient;
typedef std::function<ResponsePtr()> ResponseFactory;
//typedef std::function<bool(ResponsePtr, HttpClient*)>    RequestWriteComplete;
typedef std::function<bool(ResponsePtr&, HttpClient*)>  ResponseReady;
typedef std::function<bool(RequestBuilderPtr&, std::string&)>  RequestWrite;
typedef std::function<bool(HttpClient*)>                       ClientCB;

class HttpClient {
public:
	HttpClient(EventLoop *loop,  AddrInet &_serverAddr, std::string &name);
	~HttpClient();
	
	void connect();
	void shutdown();
	bool postRequest(RequestBuilderPtr &req);
	
	void setResponseReady(const ResponseReady &cb);
	void setRequestWrite(const RequestWrite& cb);
	void setResponseFactory(const ResponseFactory &cb);
	void setConnectCB(const ClientCB &cb) { _onConnect = cb;}
	void setCloseCB(const ClientCB &cb) { _onClose = cb; }
	void setWriteComplete(const ClientCB &cb) { _writeComplete = cb; }
	
	void setContext(const any &any);
	const any getContext();
	any *getMutableContext();
private:
	void onMessage(const TcpConnectionPtr &conn,
                              Buffer *buf,
                              Time recvtime);
	void onConnection(const TcpConnectionPtr &conn);
	void onWriteComplete(const TcpConnectionPtr &conn);

	
private:
	void sendReques();
	void sendRequestInloop();
	bool isSendComplete();
private:
	ResponseFactory              _responseFactory;
	ResponseReady                _resReady;
	RequestWrite                 _reqWrite;
	ClientCB                     _resParseError;
	ClientCB                     _onConnect;
	ClientCB                     _onClose;
	ClientCB                     _writeComplete;
private:
	Time                          _lastrecv;
	any                           _context;
	AddrInet                      _serverAddr;
	EventLoop                    *_loop;
	TcpClient                     _client;
	std::weak_ptr<TcpConnection>  _conn;
	RequestBuilderPtr             _currentRequest;
	ResponsePtr                   _currentResponse;
	BlockQueue<RequestBuilderPtr> _reqQueue;
	BlockQueue<RequestBuilderPtr> _sendQueue;
	
};
}
}
#endif