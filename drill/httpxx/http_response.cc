#include <drill/httpxx/http_response.h>
#include <string.h>

namespace drill {
namespace httpxx {

    Response::Response ()
    {
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_RESPONSE);
        _parser.data = this;
    }

    Response::Response ( Configure configure )
        : Message(configure)
    {
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_RESPONSE);
        _parser.data = this;
    }

    void Response::clear ()
    {
        Message::clear();
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_RESPONSE);
        _parser.data = this;
    }

    int Response::status () const
    {
        return (_parser.status_code);
    }

}
}

