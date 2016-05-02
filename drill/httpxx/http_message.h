#ifndef DRILL_HTTPXX_HTTP_MESSAGE_H_
#define DRILL_HTTPXX_HTTP_MESSAGE_H_

#include <drill/httpxx/http_parser.h>
#include <drill/httpxx/http_flags.h>
#include <drill/httpxx/http_error.h>

#include <map>
#include <string>
#include <functional>

namespace drill {
namespace httpxx {

    class Message {
        /* nested types. */
	public:
        typedef std::map<std::string, std::string> Headers;

    protected:
        typedef std::function<void(::http_parser_settings&)> Configure;

    protected:
        Message ( Configure configure=0 );

    public:
        virtual ~Message ();

    public:

        virtual void clear ();
        virtual void resetBuffers ();

        std::size_t feed ( const void * data, ::size_t size );

        std::size_t feed ( const char * data, ::size_t size );

        bool complete () const;

        bool headersComplete () const;

        int majorVersion () const;

        int minorVersion () const;

        const Flags flags () const;

        bool hasHeader ( const std::string& field ) const;

        std::string header ( const std::string& field ) const;

		const Headers& headers() const;

        bool shouldKeepAlive () const;
		bool         isValid();
		const Error& getError() const;
    private:
        Message& operator= ( const Message& );
		Message ( const Message& );
        static int on_message_begin ( ::http_parser * parser );

        static int on_message_complete ( ::http_parser * parser );

        static int on_header_field
            ( ::http_parser * parser, const char * data, size_t size );

        static int on_header_value
            ( ::http_parser * parser, const char * data, size_t size );

        static int on_headers_complete ( ::http_parser * parser );
	protected:
        ::http_parser_settings _settings;
        ::http_parser          _parser;
		Error                  _error; 

        std::string _currentField;
        std::string _currentValue;
        std::map<std::string, std::string> _headers;
        bool _complete;
        bool _headersComplete;
    };

}
}
#endif 
