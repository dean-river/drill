#include <drill/http/http_response.h>
#include <algorithm>
#include <drill/common/string.h>

namespace drill {

namespace http {

  HttpResponse::HttpResponse():
  					parseStat(e_stateline),
   					need_do_size(0),
   					had_do_size(0),
   					httpcode(0)
  					
  {
  }
  HttpResponse::~HttpResponse()
  {

  }
  void HttpResponse::recvStatLine()
  {
	parseStat = e_header;
  }
  void HttpResponse::recvHeader()
  {
	parseStat = e_body;
  }

  void HttpResponse::recvBody()
  {
	parseStat = g_all;
  }
  bool HttpResponse::isvalid()
  {
	return parseStat != e_error;
  }

  bool HttpResponse::getAll()
  {
	return parseStat == g_all;
  }
  size_t HttpResponse::parseResponse(const char* buff, size_t len)
  {
  		size_t parseSize = 0;
		size_t segsize; 
		if( parseStat == e_stateline) {
			segsize = parseStatLine(buff, len);
			parseSize +=  segsize;

		}		

		if(parseStat == e_header) {
			segsize = parseHeaders(buff + parseSize , len - parseSize);
			parseSize +=  segsize;
		}

		if(parseStat == e_body) {
			segsize = parseBody(buff + parseSize,len - parseSize);
			parseSize +=  segsize;
		}
		return parseSize;
  }


  size_t	HttpResponse::parseStatLine(const char* buff, size_t len)
  {
		assert(buff);
		size_t parsed = 0;
		const char *c = "\r\n";
		const char * end = buff+len;
		const char *crlf = std::search(buff, end, c, c+2);
		if(crlf == end ) {
			return 0;
		}
		//version
		parsed = crlf - buff;
		const char *space = std::find(buff, crlf, ' ');
		
		version =string(buff,space);
		
		while(*space == ' ') space++;
		const char *statmsg = std::find(space, crlf, ' ');
		httpcode = drill::common::string_to_int(space,(size_t)(statmsg - space));
		
		while(*statmsg == ' ') statmsg++;
		
		httpStatline = string(statmsg, crlf);
		recvStatLine();
		return parsed+2;
		
  }
  size_t	HttpResponse::parseHeaders(const char* buff, size_t len)
  {
		const char *end = buff+len;
		const char *c = "\r\n";
		const char *crlf = std::search(buff, end, c, c+2);
		const char *start = buff;
		size_t parsed = 0;
		while (crlf != end) {
			const char* colon = std::find(start, crlf, ':');
			if(colon == crlf) {
				recvHeader();
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
void HttpResponse::preParseBody()
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
size_t	HttpResponse::parseBody(const char* buff, size_t len)
{
	size_t parsed;
	if(len <= need_do_size- had_do_size) {
		parsed = len;
	} else {
		parsed = need_do_size- had_do_size;
	}
	had_do_size += parsed;
	body.append(buff,parsed);
	if(need_do_size == had_do_size) {
		recvBody();
	}
	return parsed;
}
}
}
