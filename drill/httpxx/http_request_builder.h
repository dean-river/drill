#ifndef DRILL_HTTPXX_HTTP_REQUEST_BUILDER_H_
#define DRILL_HTTPXX_HTTP_REQUEST_BUILDER_H_

#include <drill/httpxx/http_message_builder.h>
#include <drill/httpxx/http_request.h>
#include <memory>

namespace drill {
namespace httpxx {
	class RequestBuilder : public MessageBuilder
	{
	public:
		RequestBuilder();
		RequestBuilder(const Request& request);

		/*!
		 * @brief Obtain the HTTP method used by the client for the request.
		 * @return One of the enumeration values.
		 */
		const Method method() const;

		/*!
		 * @brief Get the HTTP method as a string.
		 */
		std::string methodName() const;

		/*!
		 * @brief Set the HTTP method used by the client for the request.
		 * @param method The HTTP method.
		 */
		void setMethod(const Method& method);
		
		/*!
		 * @brief Check if client requested a connection upgrade.
		 */
		bool upgrade() const;

		/*!
		 * @brief Sets if client requested a connection upgrade.
		 * @param upgrade The value of the upgrade flag.
		 */
		void setUpgrade(bool upgrade);

		/*!
		 * @brief Get the request URL.
		 */
		const std::string& url() const;

		/*!
		 * @brief Sets the request URL.
		 * @param url The request URL.
		 */
		void setUrl(const std::string& url);

		/*!
		 * @brief Gets a string representation of the request line.
		 */
		std::string requestLine() const;

		/*!
		 * @brief Gets a full string representation of the request.
		 */
		std::string toString() const;

	private:
		Method _method;
		std::string _url;
	};

	typedef std::shared_ptr<RequestBuilder> RequestBuilderPtr;
}
}
#endif /* _http_Request_builder__ */