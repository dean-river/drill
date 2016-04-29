#ifndef DRILL_HTTPXX_HTTP_MESSAGE_BUILDER_H_
#define DRILL_HTTPXX_HTTP_MESSAGE_BUILDER_H_

#include <drill/httpxx/http_message.h>

namespace drill {
namespace httpxx {
	class MessageBuilder
	{
	public:
		MessageBuilder();
		MessageBuilder(const Message& message);

		int majorVersion() const;
		void setMajorVersion(int majorVersion);

		/*!
		 * @brief Obtain the HTTP revision used by the remote peer.
		 * @return A numeric error code (should be 0 or 1 for now).
		 */
		int minorVersion() const;

		/*!
		 * @brief Set the HTTP revision used by the remote peer.
		 * @param minorVersion The HTTP revision
		 */
		void setMinorVersion(int minorVersion);

		/*!
		 * @brief Obtain parser flags.
		 * @return Flags documenting the parsed request/response object.
		 */
		const Flags& flags() const;

		/*!
		 * @brief Obtain reference to parser flags.
		 * @return Flags documenting the parsed request/response object.
		 */
		Flags& flags();

		/*!
		 * @brief Set parser flags.
		 * @param flags Flags documenting the parsed request/response object.
		 */
		void setFlags(const Flags& flag);

		/*!
		 * @brief Returns a reference to the headers map
		 */
		Message::Headers& headers();

		/*!
		* @brief Returns a reference to the headers map
		*/
		const Message::Headers& headers() const;

		/*!
		 * @brief Returns a string representation of the HTTP version
		 */
		std::string versionToString() const;

		/*!
		 * @brief Returns the string representation of the message headers
		 */
		std::string headersToString() const;

	private:
		int             _major_version;
		int             _minor_version;
		Flags            _flags;
		Message::Headers _headers;
	};
}
}

#endif /* _http_MessageBuilder__ */
