#include <drill/httpxx/http_request_builder.h>

namespace drill {
namespace httpxx {
	RequestBuilder::RequestBuilder() : 
		MessageBuilder(), _method(Method::get())
	{

	}

	RequestBuilder::RequestBuilder(const Request& request) : 
		MessageBuilder(request), _method(Method::get())
	{
		if(request.headersComplete())
		{
			_method = request.method();
			_url = request.url();
		}
	}

	const Method RequestBuilder::method() const
	{
		return _method;
	}

	std::string RequestBuilder::methodName() const
	{
		return _method.toString();
	}

	void RequestBuilder::setMethod(const Method& m)
	{
		_method = m;
	}

	bool RequestBuilder::upgrade() const
	{
		return flags() & Flags::upgrade();
	}

	void RequestBuilder::setUpgrade(bool ug)
	{
		flags().setUpgrade(ug);
	}

	const std::string& RequestBuilder::url() const
	{
		return _url;
	}

	void RequestBuilder::setUrl(const std::string& u)
	{
		_url = u;
	}

	std::string RequestBuilder::requestLine() const
	{
		std::string request_line(method().toString());
		request_line += " "; 
		request_line += url();
		request_line += " ";
		request_line += versionToString();
		request_line += "\r\n";
		
		return request_line;
	}

	std::string RequestBuilder::toString() const
	{
		std::string str(requestLine());
		str += headersToString();
		str += "\r\n";
		
		return str;
	}
}
}