#include <drill/httpxx/http_response_builder.h>
#include <stdio.h>

struct StatusCodesStruct
{
	int code;
	const char* reason;
};

static const StatusCodesStruct gStatusCodesStruct[] = 
{
	{ 100, "Continue" },
	{ 101, "Switching Protocols" },
	{ 200, "OK" },
	{ 201, "Created" },
	{ 202, "Accepted" },
	{ 203, "Non-Authoritative Information" },
	{ 204, "No Content" },
	{ 205, "Reset Content" },
	{ 206, "Partial Content" },
	{ 300, "Multiple Choices" },
	{ 301, "Moved Permanently" },
	{ 302, "Found" },
	{ 303, "See Other" },
	{ 304, "Not Modified" },
	{ 305, "Use Proxy" },
	{ 307, "Temporary Redirect" },
	{ 400, "Bad Request" },
	{ 401, "Unauthorized" },
	{ 402, "Payment Required" },
	{ 403, "Forbidden" },
	{ 404, "Not Found" },
	{ 405, "Method Not Allowed" },
	{ 406, "Not Acceptable" },
	{ 407, "Proxy Authentication Required" },
	{ 408, "Request Time-out" },
	{ 409, "Conflict" },
	{ 410, "Gone" },
	{ 411, "Length Required" },
	{ 412, "Precondition Failed" },
	{ 413, "Request Entity Too Large" },
	{ 414, "Request-URI Too Large" },
	{ 415, "Unsupported Media Type" },
	{ 416, "Requested range not satisfiable" },
	{ 417, "Expectation Failed" },
	{ 500, "Internal Server Error" },
	{ 501, "Not Implemented" },
	{ 502, "Bad Gateway" },
	{ 503, "Service Unavailable" },
	{ 504, "Gateway Time-out" },
	{ 505, "HTTP Version not supported" }
};

static const int gStatusCodesStructSize = sizeof(gStatusCodesStruct) / sizeof(StatusCodesStruct);

struct StatusCodes
{
	std::map<int, std::string> codes;

	StatusCodes()
	{
		for(int i = 0; i < gStatusCodesStructSize; ++i)
		{
			codes[gStatusCodesStruct[i].code] = gStatusCodesStruct[i].reason;
		}
	}
};

static StatusCodes gStatusCodes;

namespace drill {
namespace httpxx {
	ResponseBuilder::ResponseBuilder() : 
		MessageBuilder(), _status(400), _close(true)
	{

	}

	ResponseBuilder::ResponseBuilder(const Response& response) :
		MessageBuilder(response), _status(400)
	{
		if(response.headersComplete())
		{
			_status = response.status();
		}
	}

	int ResponseBuilder::status() const
	{
		return _status;
	}

	std::string ResponseBuilder::statusReason() const
	{
		std::map<int, std::string>::const_iterator cit = gStatusCodes.codes.find(status());
		if(cit != gStatusCodes.codes.end())
		{
			return cit->second;
		}

		return "";
	}

	void ResponseBuilder::setStatus(int stat)
	{
		_status = stat;
	}

	std::string ResponseBuilder::statusLine() const
	{
		char statusBuf[16];

		snprintf(statusBuf, 16, "%d", status());

		std::string status_line(versionToString());
		status_line += " ";
		status_line += statusBuf;
		status_line += " ";
		status_line += statusReason();
		status_line += "\r\n";

		return status_line;
	}

	std::string ResponseBuilder::toString() const
	{
		std::string str(statusLine());
		str += headersToString();
		str += "\r\n";
		return str;
	}
	bool ResponseBuilder::isSendComplete()
	{
		return true;
	}
	void ResponseBuilder::setClose(bool isclose)
	{
		_close = isclose;
	}

	bool ResponseBuilder::ifClose()
	{
		return _close;
	}
	bool ResponseBuilder::sendMessage(std::string &mes)
	{
		mes += statusLine();
		mes += headersToString();
		mes += "\r\n";
		return true;
	}

}
}
