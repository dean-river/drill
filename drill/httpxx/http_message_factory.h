#ifndef DRILL_HTTPXX_HTTP_MESSAGE_FACTORY_H_
#define DRILL_HTTPXX_HTTP_MESSAGE_FACTORY_H_
#include <drill/httpxx/http_response.h>
#include <drill/httpxx/http_request.h>

namespace drill {
namespace httpxx {

class MessageFactor {
public:
	virtual ~MessageFactor() {}

	RequestPtr createRequest() = 0;

	ResponsePtr createResposne() = 0 ;
	
};

}
}
#endif
