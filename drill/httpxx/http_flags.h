#ifndef DRILL_HTTPXX_HTTP_FLAGS_H_
#define DRILL_HTTPXX_HTTP_FLAGS_H_

#include <drill/httpxx/http_parser.h>

namespace drill {
namespace httpxx {

class Flags {
    public:
        typedef ::flags Value;
    public:
		Flags();
		Flags(const Flags& flags);

		void setChunked(bool ischunked);
		void setKeepalive(bool iskeepalive);
		void setClose(bool isclose);
		void setTrailing(bool istrailing);
		void setUpgrade(bool isupgrade);
		void setSkipbody(bool isskipbody);

	public:

        static const Flags of ( const ::http_parser& parser );

        static const Flags chunked ();

        static const Flags keepalive ();


        static const Flags close ();

        static const Flags trailing ();

        static const Flags upgrade ();

        static const Flags skipbody ();


    private:
        Flags ( Value value );

    public:

        bool operator& ( const Flags& rhs ) const;

		Flags operator| ( const Flags& rhs ) const;

		Flags operator~ () const;
	private:
        Value _value;
    };

}
}

#endif 
