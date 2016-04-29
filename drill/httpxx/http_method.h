#ifndef _http_Method__
#define _http_Method__
#include <drill/httpxx/http_parser.h>


#include <string>
namespace drill {
namespace httpxx {

class Method {
    private:
        typedef ::http_method Value;

    public:

        static const Method of ( const ::http_parser& parser );

        static const Method del ();

        static const Method get ();

        static const Method head ();

        static const Method post ();

        static const Method put  ();


        static const Method patch ();

        static const Method connect ();

        static const Method options ();

        static const Method trace ();


        static const Method copy ();
        static const Method lock ();
        static const Method mkcol ();
        static const Method move ();
        static const Method propfind ();
        static const Method proppatch ();
        static const Method unlock ();

        static const Method report ();
        static const Method mkactivity ();
        static const Method checkout ();
        static const Method merge ();

        static const Method msearch ();
        static const Method notify ();
        static const Method subscribe ();
        static const Method unsubscribe ();

		std::string toString() const;


    private:
        Value _value;

    private:
        Method ( Value value );

    public:
        bool operator== ( const Method& rhs ) const;
        bool operator!= ( const Method& rhs ) const;

        operator Value () const;
    };

}
}
#endif
