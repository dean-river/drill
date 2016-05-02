#ifndef DRILL_HTTPXX_HTTP_RESPONSE_BUILDER_H_
#define DRILL_HTTPXX_HTTP_RESPONSE_BUILDER_H_

#include <drill/httpxx/http_message_builder.h>
#include <drill/httpxx/http_response.h>
#include <memory>
namespace drill {
namespace httpxx {
	class ResponseBuilder : public MessageBuilder
	{
	public:
		ResponseBuilder();
		ResponseBuilder(const Response& response);

		/*!
		 * @brief Gets the HTTP status code of the response.
		 */
		int status() const;

		/*!
		 * @brief Gets the reason string of the HTTP status code.
		 */
		std::string statusReason() const;

		/*!
		 * @brief Sets the HTTP status code.
		 * @param status The HTTP status code.
		 */
		void setStatus(int stat);

		/*!
		 * Gets a string representation of the HTTP response status line.
		 */
		std::string statusLine() const;

		/*
		 * Gets a full string representation of the HTTP response.
		 */
		std::string toString() const;

		virtual bool isSendComplete();

		virtual bool sendMessage(std::string &mes);
		
	private:
		int _status;
	};
	typedef std::shared_ptr<ResponseBuilder> ResponseBuilderPtr;
}
}
#endif 