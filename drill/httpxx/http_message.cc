#include <drill/httpxx/http_message.h>
#include <drill/httpxx/http_error.h>

#include <algorithm>
#include <cstring>
#include <utility>

#include <iostream>

namespace drill {
namespace httpxx {

	struct Clear {
        typedef std::pair<const std::string, std::string> Header;
        void operator() ( Header& header ) const { header.second.clear(); }
    };
	
	class Matches
    {
        const std::string &_field;
    public:
        Matches ( const std::string& field )
            : _field(field)
        {}
        bool operator() ( const std::pair<std::string,std::string>& field )
        {
            return field.first == _field;
        }
    };

    int Message::on_message_begin ( ::http_parser * parser )
    {
        Message& message = *static_cast<Message*>(parser->data);
        message._complete = false;
        message._headersComplete = false;
        return (0);
    }

    int Message::on_message_complete ( ::http_parser * parser )
    {
        Message& message = *static_cast<Message*>(parser->data);
        message._complete = true;
        http_parser_pause(parser, 1);

        return (0);
    }

    int Message::on_header_field
        ( ::http_parser * parser, const char * data, size_t size )
    {
        Message& message = *static_cast<Message*>(parser->data);
        if ( !message._currentValue.empty() )
        {
            message._headers[message._currentField] =
                message._currentValue;
            message._currentField.clear();
            message._currentValue.clear();
        }
        message._currentField.append(data, size);
        return (0);
    }

    int Message::on_header_value
        ( ::http_parser * parser, const char * data, size_t size )
    {
        Message& message = *static_cast<Message*>(parser->data);
        message._currentValue.append(data, size);
        return (0);
    }

    int Message::on_headers_complete ( ::http_parser * parser )
    {
        Message& message = *static_cast<Message*>(parser->data);
        if ( !message._currentValue.empty() )
        {
            message._headers[message._currentField] =
            message._currentValue;
            message._currentField.clear();
            message._currentValue.clear();
        }
        message._headersComplete = true;

        // Force the parser to stop after the headers are parsed so clients
        // can process the request (or response).  This is to properly
        // handle HTTP/1.1 pipelined requests.
        http_parser_pause(parser, 1);

        return (0);
    }

    Message::Message ( Configure configure )
    {
            // make sure message is not seen as complete.
        _complete = false;
        _headersComplete = false;
            // select callbacks.
        ::memset(&_settings, 0, sizeof(_settings));
        _settings.on_message_complete = &Message::on_message_complete;
        _settings.on_message_begin    = &Message::on_message_begin;
        _settings.on_header_field     = &Message::on_header_field;
        _settings.on_header_value     = &Message::on_header_value;
        _settings.on_headers_complete = &Message::on_headers_complete;
        if (configure) {
            configure(_settings);
        }
    }

    Message::~Message ()
    {
    }

    void Message::clear ()
    {
            // make sure message is not seen as complete.
        _complete = false;
        _headersComplete = false;
            // clear string content, while keeping memory allocated.
        std::for_each(_headers.begin(), _headers.end(), Clear());
    }

    void Message::resetBuffers ()
    {
        std::map<std::string,std::string> empty;
        _headers.swap(empty);
    }

    std::size_t Message::feed ( const void * data, ::size_t size )
    {
        return (feed(static_cast<const char*>(data), size));
    }

    std::size_t Message::feed ( const char * data, ::size_t size )
    {
        std::size_t used = ::http_parser_execute
            (&_parser, &_settings, data, size);

        const ::http_errno error =
            static_cast< ::http_errno >(_parser.http_errno);
        if (error == HPE_PAUSED) {
            ::http_parser_pause(&_parser, 0);
        }

        if (used < size)
        {
            if (error == HPE_PAUSED)
            {
                if (!_complete)
                {
                    used += ::http_parser_execute
                        (&_parser, &_settings, data+used, 1);
                }
            }
            else {
             	_error.set(error);
            }
        }

        return (used);
    }

    bool Message::complete () const
    {
        return (_complete);
    }

    bool Message::headersComplete () const
    {
        return (_headersComplete);
    }

    int Message::majorVersion () const
    {
        return (_parser.http_major);
    }

    int Message::minorVersion () const
    {
        return (_parser.http_minor);
    }

    const Flags Message::flags () const
    {
        return (Flags::of(_parser));
    }

    bool Message::hasHeader ( const std::string& field ) const
    {
        const Headers::const_iterator match =
            std::find_if(_headers.begin(), _headers.end(), Matches(field));
        return ((match != _headers.end()) && !match->second.empty());
    }

    std::string Message::header ( const std::string& field ) const
    {
        const Headers::const_iterator match =
            std::find_if(_headers.begin(), _headers.end(), Matches(field));
        if ( match == _headers.end() ) {
            return ("");
        }
        return (match->second);
    }

	const Message::Headers& Message::headers() const
	{
		return _headers;
	}

    bool Message::shouldKeepAlive () const
    {
        return (::http_should_keep_alive
                (const_cast< ::http_parser* >(&_parser)) != 0);
    }

	bool       Message:: isValid()
	{
		return _error.isValid();
	}
	const Error& Message::getError() const
	{
		return _error;
	}

}
}
