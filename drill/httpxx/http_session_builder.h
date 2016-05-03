#ifndef DRILL_HTTPXX_HTTP_SESSION_BUILDER_H_
#define DRILL_HTTPXX_HTTP_SESSION_BUILDER_H_
#include <drill/httpxx/http_server_session.h>
namespace drill {
namespace httpxx {

class SessionBuilder {
public:
	virtual ~SessionBuilder() {}

	virtual bool build(HttpServerSessionPtr &s)  { (void)s; return true; }
	virtual bool clear(HttpServerSessionPtr &s)  { (void)s;  return true; }
};

}
}
#endif