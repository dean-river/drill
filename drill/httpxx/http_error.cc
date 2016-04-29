#include <drill/httpxx/http_error.h>

namespace drill {
namespace httpxx {

    Error::Error ( ::http_errno number )
        : _errnumber(number)
    {
    }

    const char * Error::what () const throw()
    {
        return (::http_errno_description(_errnumber));
    }
	bool     Error::isValid()
	{
		return _errnumber == HPE_OK;
	}

	void     Error::set(::http_errno number)
	{
		_errnumber = number;
	}
}
}
