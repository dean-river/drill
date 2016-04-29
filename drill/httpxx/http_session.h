#ifndef DRILL_HTTPXX_HTTP_SESSION_H_
#define DRILL_HTTPXX_HTTP_SESSION_H_
#include <drill/common/any.h>
#include <drill/common/buffer.h>
#include <drill/common/time.h>
#include <drill/httpxx/http_buffered_message.h>
#include <string>
#include <list>

using drill::common::any;
using drill::common::Time;
using drill::common::Buffer;

namespace drill {
namespace httpxx {

class HttpSession {
public:
	HttpSession();
	virtual ~HttpSession();

	void setContext(any & c);
	
	bool parseRequest(Buffer *buf, Time &recvtime);

	std::string processBadRequest();

	bool getCurrentRequest();

	bool handleRequest();

	bool closeConnection();
	
private:
	bool              _close;
	Time              _lastRecv;
	any               _context;
	BufferedRequest  *_currentRequest;
	BufferedResponse *_currentResponse;
	std::list<BufferedRequest*> _requestList;
};

}
}
#endif