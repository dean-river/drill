#include <drill/httpxx/http_request.h>
#include <string.h>

namespace drill {
namespace httpxx {

    int Request::on_url
        ( ::http_parser * parser, const char * data, size_t size )
    {
        Request& request = *static_cast<Request*>(parser->data);
        request._url.append(data, size);
        return (0);
    }

    Request::Request ()
    {
            // initialize parser.
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_REQUEST);
        _parser.data = this;
            // select callbacks.
        _settings.on_url          = &Request::on_url;
    }

    Request::Request ( Configure configure )
        : Message(configure)
    {
            // initialize parser.
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_REQUEST);
        _parser.data = this;
            // select callbacks.
        _settings.on_url = &Request::on_url;
    }

    void Request::clear ()
    {
        Message::clear();
            // clear string content, while keeping memory allocated.
        _url.clear();
            // (re-)initialize parser.
        ::memset(&_parser, 0, sizeof(_parser));
        ::http_parser_init(&_parser, HTTP_REQUEST);
        _parser.data = this;
    }

    void Request::resetBuffers ()
    {
        std::string empty;
        _url.swap(empty), Message::resetBuffers();
    }

    const Method Request::method () const
    {
        return (Method::of(_parser));
    }

    std::string Request::methodName () const
    {
        return (http_method_str(method()));
    }

    bool Request::upgrade () const
    {
        return (_parser.upgrade != 0);
    }

    const std::string& Request::url () const
    {
        return (_url);
    }

}
}
