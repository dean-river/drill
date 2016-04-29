#ifndef DRILL_HTTPXX_HTTP_ERROR_H_
#define DRILL_HTTPXX_HTTP_ERROR_H_
 
#include <drill/httpxx/http_parser.h>
namespace drill {
namespace httpxx {

class Error {


    public:

        explicit Error ( ::http_errno number = HPE_OK);

		bool     isValid();

		void     set(::http_errno number);
    public:

        virtual const char * what () const throw();
        ::http_errno _errnumber;
    };

}
}

#endif
