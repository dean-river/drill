#ifndef DRILL_HTTPXX_HTTP_MESSAGE_FACTORY_H_
#define DRILL_HTTPXX_HTTP_MESSAGE_FACTORY_H_
#include <drill/httpxx/http_response.h>
#include <drill/httpxx/http_request.h>
#include <drill/httpxx/http_response_builder.h>
#include <drill/httpxx/http_request_builder.h>

namespace drill {
namespace httpxx {

class MessageFactor {
public:
	virtual ~MessageFactor() {}

	RequestPtr createRequest() { return RequestPtr();}

	ResponsePtr createResposne() { return ResponsePtr(); }

	RequestBuilderPtr createRequestBuilder() { return RequestBuilderPtr();}

	ResponseBuilderPtr createResposneBuilder() { return ResponseBuilderPtr(); }
	
	
};

}
}
#endif
