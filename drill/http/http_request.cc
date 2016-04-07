#include <drill/http/http_request.h>
#include <drill/common/slice.h>
#include <algorithm>
#include <drill/common/string.h>

using std::string;

namespace drill {
namespace http {

HttpRequest::HttpRequest()
:parse_status(e_request),
 encoding(kPlain)
{

}

size_t HttpRequest::parseRequest(const char *data, size_t len)
{
	switch (parse_status) {
		case e_request:
			return parseRequestLine(data, len);
		case e_header:
			return parseHeaders(data, len);
		case e_body:
			return parseBody(data, len);
		default:
			break;
	}

	return 0;
}

  size_t HttpRequest::parseRequestLine(const char* buff, size_t len)
  {
		assert(buff);
		size_t parsed = 0;
		const char *c = "\r\n";
		const char *ws = " ";
		const char *help = "?";
		const char * end = buff+len;
		const char *crlf = std::search(buff, end, c, c+2);
		if(crlf == end ) {
			return 0;
		}
	
		parsed = crlf - buff;
		//Slice line(buff, len);
		//method
		const char *space = std::search(buff, crlf, ws, ws+1);
		if(space == crlf) {
			parse_status = e_error;
			return 0;
		}
		method.append(buff,space);
		//uri
		while(*space == ' ') space++;
		
		const char *uri_end = std::search(space, crlf, ws, ws + 1);
		if(space == crlf) {
			parse_status = e_error;
			return 0;
		}
		
		const char *args_end = std::search(space, uri_end, help, help + 1);
		uri.set(Slice(space, args_end - space));
		//have args
		if(args_end != uri_end) {
			args_end++;
			if(!parseArgs(args_end, uri_end - args_end)) {
				parse_status = e_error;
				return 0;	
			}
			
		}
		space = uri_end;
		//version
		while (space != crlf && *space == ' ') space++;
		if(space == crlf) {
			parse_status = e_error;
			return 0;
		}
		version.append(space, crlf);
		recvRequestLine();
		return parsed+2;
		
  }
	
  size_t HttpRequest::parseHeaders(const char* buff, size_t len)
  {
		const char *end = buff+len;
		const char *c = "\r\n";
		const char *crlf = std::search(buff, end, c, c+2);
		const char *start = buff;
		size_t parsed = 0;
		while (crlf != end) {
			const char* colon = std::find(start, crlf, ':');
			if(colon == crlf) {
				recvheader();
				preParseBody();
				parsed += 2;
				return parsed;
			}
			string key(start,colon);
			colon++;
			while(*colon == ' ') colon++;
			string value(colon,crlf);
			while(value[value.size()-1] == ' ') {
				size_t valuesize = value.size();
				value.resize(valuesize -1);
			}
			header[key] = value;
			parsed += crlf - start + 2;
			start = crlf + 2;
			
			crlf = std::search(start, end, c, c+2);
			
		}
		return parsed;
  }

bool HttpRequest::parseArgs(const char* buff, size_t len)
{
	const char *andchar = "&";
	const char *equal = "=";
	const char *start = buff;
	const char *buff_end = buff + len;
	const char *end = std::search(start, buff_end, andchar, andchar+1);
	const char *split = end;
	while(end != buff_end) {
		split = std::search(start, end, equal, equal + 1); 
		if(split == end) {
			return false;
		}	
		std::string key(start,split);
		std::string value(split+1, end);
		args[key] = value;

		start = end+1;
		end = std::search(start, buff_end, andchar, andchar+1);
	}
	split = std::search(start, end, equal, equal + 1); 
	if(split == end) {
		return false;
	}	
	std::string key(start,split);
	std::string value(split+1, end);
	args[key] = value;
	return true;
}

void HttpRequest::preParseBody()
{
	const char* contentLen = "Content-Length";
	DictItr itr= header.find(contentLen);
	if(itr == header.end()) {
		//no content
		need_do_size = 0;
		recvBody();
		return;
	}
	need_do_size = drill::common::string_to_int(itr->second.c_str(),itr->second.length());
}
size_t	HttpRequest::parseBody(const char* buff, size_t len)
{
	size_t parsed = 0;	
	if(len <= need_do_size - had_do_size) {
		parsed = len;
	} else {
		parsed = need_do_size - had_do_size;
	}
	had_do_size += parsed;
	body.append(buff,parsed);
	if(body.size() == need_do_size) {
		recvBody();
	}
	return parsed;
}
}
}
