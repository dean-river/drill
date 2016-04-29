#ifndef DRILL_HTTPXX_HTTP_BUFFERED_MESSAGE_H_
#define DRILL_HTTPXX_HTTP_BUFFERED_MESSAGE_H_


#include <string>
#include <drill/httpxx/http_request.h>
#include <drill/httpxx/http_response.h>
#include <functional>

namespace drill {
namespace httpxx {

    template<class Base>
    class BufferedMessage : public Base {
    private:
        typedef BufferedMessage<Base> Self;

        /* class methods. */
    private:
        static int on_body
            ( ::http_parser * parser, const char * data, size_t size )
        {
            static_cast<Self*>(parser->data)->_body.append(data, size);
            return (0);
        }

        static void extra_configuration ( ::http_parser_settings& settings )
        {
            settings.on_body = &Self::on_body;
        }

        /* construction. */
    public:
        BufferedMessage ()
            : Base(std::bind(&Self::extra_configuration, std::placeholders::_1))
        {
        }

        /* data. */
    private:
        std::string _body;

        /* methods. */
    public:
        /*!
         * @brief Obtain the buffered message body.
         */
        const std::string& body () const
        {
            return (_body);
        }

        /*!
         * @brief Clear the body contents after processing them.
         *
         * This method can be used between calls to @c feed() once @c
         * headers_complete() returns @c true, allowing you to process the
         * body in chunks at your convenience.
         */
        void clearBody ()
        {
            _body.clear();
        }

        /*!
         * @brief Empty all message content, but keep allocated buffers.
         */
        virtual void clear ()
        {
            _body.clear(), Base::clear();
        }

        /*!
         * @brief Release memory owned by all internal buffers.
         */
        virtual void resetBuffers ()
        {
            std::string().swap(_body), Base::resetBuffers();
        }
    };

    typedef BufferedMessage<Request> BufferedRequest;
    typedef BufferedMessage<Response> BufferedResponse;

}
}
#endif 
