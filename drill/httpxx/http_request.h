#ifndef DRILL_HTTPXX_HTTP_REQUEST_H_
#define DRILL_HTTPXX_HTTP_REQUEST_H_

#include <drill/httpxx/http_message.h>
#include <drill/httpxx/http_method.h>
#include <memory>

namespace drill {
namespace httpxx {

    class Request : public Message {

    public:
            /*!
             * @brief Build a fresh, independant HTTP request parser.
             */
        Request ();

    protected:
        Request ( Configure configure );

        /* methods. */
    public:
            /*!
             * @brief Empty all request content, but keep allocated buffers.
             */
        virtual void clear ();

            /*!
             * @brief Release memory owned by all internal buffers.
             */
        virtual void resetBuffers ();

            /*!
             * @brief Obtain the HTTP method used by the client for the request.
             * @return One of a few enumeration values.
             *
             * @note There are a variety of methods other than GET and POST.
             *  Make sure to validate that this corresponds to one of the HTTP
             *  methods you plan to support.
             */
        const Method method () const;

            /*!
             * @brief Get the HTTP method as a string.
             */
        std::string methodName () const;

            /*!
             * @brief Check if the client requested a connection upgrade.
             *
             * @warning This value is unspecified until @c headerscomplete()
             *  returns @c true.
             */
        bool upgrade () const;

            /*!
             * @brief Get the request URL.
             *
             * @warning This value is unspecified until @c headerscomplete()
             *  returns @c true.
             */
        const std::string& url () const;
			
	private:
		std::string _url;

	private:
        static int on_url
            ( ::http_parser * parser, const char * data, size_t size );
    };

typedef std::shared_ptr<Request> RequestPtr;
}

}

#endif /* _http_Request__ */
