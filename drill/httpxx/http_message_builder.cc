#include <drill/httpxx/http_message_builder.h>

#include <stdio.h>

namespace drill {
namespace httpxx {
	MessageBuilder::MessageBuilder() :
		_major_version(1), _minor_version(0)
	{

	}

	MessageBuilder::MessageBuilder(const Message& message) : 
		_major_version(1), _minor_version(0)
	{
		if(message.headersComplete())
		{
			_major_version = message.majorVersion();
			_minor_version = message.minorVersion();
			_flags = message.flags();
			_headers = message.headers();
		}
	}

	int MessageBuilder::majorVersion() const
	{
		return _major_version;
	}

	void MessageBuilder::setMajorVersion(int major_version)
	{
		_major_version = major_version;
	}

	int MessageBuilder::minorVersion() const
	{
		return _minor_version;
	}

	void MessageBuilder::setMinorVersion(int minor_version)
	{
		_minor_version = minor_version;
	}

	const Flags& MessageBuilder::flags() const
	{
		return _flags;
	}

	Flags& MessageBuilder::flags()
	{
		return _flags;
	}

	void MessageBuilder::setFlags(const Flags& flag)
	{
		_flags = flag;
	}

	Message::Headers& MessageBuilder::headers()
	{
		return _headers;
	}

	const Message::Headers& MessageBuilder::headers() const
	{
		return _headers;
	}

	std::string MessageBuilder::versionToString() const
	{
		std::string versionString("HTTP/");
		char major_version[16] = {0};
		char minor_version[16] = {0};

		snprintf(major_version, 16, "%d", majorVersion());
		snprintf(minor_version, 16, "%d", minorVersion());

		versionString += major_version;
		versionString += ".";
		versionString += minor_version;

		return versionString;
	}

	std::string MessageBuilder::headersToString() const
	{
		std::string headersString;
		for(Message::Headers::const_iterator cit = _headers.begin(); cit != _headers.end(); ++cit)
		{
			headersString += cit->first;
			headersString += ": ";
			headersString += cit->second;
			headersString += "\r\n";
		}

		return headersString;
	}
}
}
