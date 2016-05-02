#ifndef DRILL_HTTPXX_HTTP_RESPONSE_H_
#define DRILL_HTTPXX_HTTP_RESPONSE_H_

#include <drill/httpxx/http_message.h>
#include <memory>

namespace drill {
namespace httpxx {

    class Response : public Message {
    public:
		Response ();

    protected:
        Response ( Configure configure );

    public:
        virtual void clear ();
        int status () const;

    };
typedef std::shared_ptr<Response> ResponsePtr;
}
}

#endif /* _http_Response__ */
