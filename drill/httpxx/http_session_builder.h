#ifndef DRILL_HTTPXX_HTTP_SESSION_BUILDER_H_
#define DRILL_HTTPXX_HTTP_SESSION_BUILDER_H_
#include <drill/httpxx/http_session.h>
namespace drill {
namespace httpxx {

class SessionBuilder {
public:
	virtual ~SessionBuilder() {}

	virtual bool build(HttpSession  *s) = 0;
};

}
}
#endif