#ifndef DRILL_HTTPXX_HTTP_URL_H_
#define DRILL_HTTPXX_HTTP_URL_H_

#include <drill/httpxx/http_parser.h>

#include <string>

namespace drill {
namespace httpxx {

    class Url
    {

    public:
        Url ( const std::string& url, bool isConnect = false );
        Url ( const char * data, std::size_t size, bool isConnect = false );

    public:

		bool valid() const;
        bool hasSchema () const;
        std::string schema () const;

        bool hasHost () const;
        std::string host () const;

        bool hasPort () const;
        std::string port () const;

        bool hasPath () const;
        std::string path () const;

        bool hasQuery () const;
        std::string query () const;

        bool hasFragment () const;
        std::string fragment () const;

		std::string absolutePath() const;
		std::string pathAndQuery() const;

    private:
        bool hasField ( http_parser_url_fields field ) const;
        std::string getField ( http_parser_url_fields field ) const;
    private:
        std::string       _data;
        ::http_parser_url _fields;
		bool              _valid;

        
    };

}
}

#endif
