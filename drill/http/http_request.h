#ifndef DRILL_HTTP_HTTPREQUEST_H_
#define DRILL_HTTP_HTTPREQUEST_H_

#include <drill/common/buffer.h>
#include <drill/common/time.h>

#include <drill/http/http_constants.h>

#include <map>
#include <cassert>
#include <cstdio>
#include <ctype.h>
#include <string>
#include <cstring>
using namespace drill::common;
using namespace std;

namespace drill {
namespace http {
class HttpRequest 
{
 public:
  HttpRequest()
    : _method(kInvalid),
      _version(kUnknown),
      _cachelen(0)
  {
  }

  void setVersion(HttpVersion v)
  {
    _version = v;
  }

  HttpVersion getVersion() const
  { return _version; }

  bool setMethod(const char* start, const char* end)
  {
    assert(_method == kInvalid);
    _method = parseMethod(string(start,end));
    return _method != kInvalid;
  }

  HttpMethod method() const
  { return _method; }

  std::string methodString() const
  {
    return getMethodString(_method).c_str();
  }

  void setPath(const char* start, const char* end)
  {
    _path.assign(start, end);
  }

  const std::string& path() const
  { return _path; }

  void setQuery(const char* start, const char* end)
  {
    _query.assign(start, end);
    parseQuery();
  }
  void parseQuery()
  {
    _args.clear();
    const char *pquery = _query.c_str();
    const char* start;
    const char * end;
    int          leave_size;
    while (*pquery == ' ' || *pquery =='?') {
        pquery++;
    }
    leave_size = strlen(pquery);
    while(leave_size) {
         start = pquery;
         end = strstr(start, "=");
         while (*end == ' ') end--;
         string key(start,end - start);
         start = end;
         while (*start == ' ' || *start == '=') {
            start++;
         }
         end = strstr(start, "&");
         if (!end) {
            end = start + strlen(start);
         }
         pquery = end;
         while (*end == ' ') end--;
         string value(start,end - start);
         _args[key] = value;
         while (*pquery == ' ' || *pquery =='&') {
            pquery++;
         }
         leave_size = strlen(pquery);
   }
  }
  const std::string& query() const
  { return _query; }

  void setReceiveTime(Time t)
  { _receiveTime = t; }

  Time receiveTime() const
  { return _receiveTime; }

  void addHeader(const char* start, const char* colon, const char* end)
  {
    string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
    {
      ++colon;
    }
    string value(colon, end);
    while (!value.empty() && isspace(value[value.size()-1]))
    {
      value.resize(value.size()-1);
    }
    _headers[field] = value;
  }
  void setClientIp(const string &ip)
  {
    _clientip = ip;
  }
  string getClientIp()
  {
       return _clientip;
  }
  string getHeader(const string& field) const
  {
    string result;
    std::map<string, string>::const_iterator it = _headers.find(field);
    if (it != _headers.end())
    {
      result = it->second;
    }
    return result;
  }
  const std::map<string, string>& args() const
  { return _args; }

  const std::map<string, string>& headers() const
  { return _headers; }

  void swap(HttpRequest& that)
  {
    std::swap(_method, that._method);
    _path.swap(that._path);
    _query.swap(that._query);
    Time tmp = _receiveTime;
    _receiveTime = that._receiveTime;
    that._receiveTime = tmp;
    _headers.swap(that._headers);
  }
  const string getbody() const
  {
	return _body;
  }
  void  setBody(Buffer *buf, size_t size)
  {
	_body.append(buf->peek(),size);
  }

  size_t getCachelen()
  {
		return _cachelen;
  }
  void setCachelen(size_t len) 
  {
	_cachelen = len;
  }
  string toString() const
  {

	string r;
   
	r.append("debug");
    r.append("clientaddr:\r\n");    
    r.append(_clientip);
     r.append("\r\n");
	r.append(methodString());
	r.append("\r\n");
	r.append(_path);
	r.append("\r\n");
	r.append(_query);
	r.append("\r\n");
	r.append(_receiveTime.toDateTime());
	r.append("\r\n");
	r.append("headers...\r\n");
	 std::map<string, string>::const_iterator itr = _headers.begin();
	for( ;itr!=_headers.end(); itr++) {
		r.append(itr->first);
		r.append(" : ");
		r.append(itr->second);
		r.append("\r\n");
	}

	itr = _args.begin();
	r.append("args...\r\n");
	for( ;itr!=_args.end(); itr++) {
		r.append(itr->first);
		r.append(" = ");
		r.append(itr->second);
		r.append("\r\n");
	}
	
	r.append("body...\r\n");
	r.append(_body);
	r.append("\r\n");
	return r;
	
  }

 private:
  string                   _clientip;
  string                   _body;
  HttpMethod               _method;
  HttpVersion              _version;
  string                   _path;
  string                   _query;
  Time                     _receiveTime;
  std::map<string, string> _headers;
  std::map<string, string> _args;
  size_t                   _cachelen;
};

}
}
#endif
