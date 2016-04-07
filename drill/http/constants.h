#ifndef DRILL_HTTP_CONSTANTS_H_
#define DRILL_HTTP_CONSTANTS_H_
#include <drill/common/slice.h>
#include <exception>
#include <map>
#include <string>
#include <vector>
#include <utility>

using namespace drill::common;

namespace drill {

namespace http {

    typedef std::map<std::string,std::string> attribute_list;
    typedef std::vector< std::pair<std::string,attribute_list> > parameter_list;

    static char const header_delimiter[] = "\r\n";

    static char const header_separator[] = ":";

    static std::string const empty_header;

    size_t const max_header_size = 16000;
    
    size_t const max_body_size = 32000000;

    size_t const istream_buffer = 512;

    static char const header_token[] = {
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..0f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 10..1f
        0,1,0,1,1,1,1,1,0,0,1,1,0,1,1,0, // 20..2f
        1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0, // 30..3f
        0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 40..4f
        1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1, // 50..5f
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // 60..6f
        1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0, // 70..7f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 80..8f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 90..9f
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // a0..af
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // b0..bf
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // c0..cf
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // d0..df
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // e0..ef
        0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // f0..ff
    };

    inline bool is_token_char(unsigned char c) {
        return (header_token[c] == 1);
    }

    inline bool is_not_token_char(unsigned char c) {
        return !header_token[c];
    }

    inline bool is_whitespace_char(unsigned char c) {
        return (c == 9 || c == 32);
    }

    inline bool is_not_whitespace_char(unsigned char c) {
        return (c != 9 && c != 32);
    }

   enum http_status_code {
          uninitialized = 0,

          continue_code = 100,
          switching_protocols = 101,

          ok = 200,
          created = 201,
          accepted = 202,
          non_authoritative_information = 203,
          no_content = 204,
          reset_content = 205,
          partial_content = 206,

          multiple_choices = 300,
          moved_permanently = 301,
          found = 302,
          see_other = 303,
          not_modified = 304,
          use_proxy = 305,
          temporary_redirect = 307,

          bad_request = 400,
          unauthorized = 401,
          payment_required = 402,
          forbidden = 403,
          not_found = 404,
          method_not_allowed = 405,
          not_acceptable = 406,
          proxy_authentication_required = 407,
          request_timeout = 408,
          conflict = 409,
          gone = 410,
          length_required = 411,
          precondition_failed = 412,
          request_entity_too_large = 413,
          request_uri_too_long = 414,
          unsupported_media_type = 415,
          request_range_not_satisfiable = 416,
          expectation_failed = 417,
          im_a_teapot = 418,
          upgrade_required = 426,
          precondition_required = 428,
          too_many_requests = 429,
          request_header_fields_too_large = 431,

          internal_server_error = 500,
          not_implemented = 501,
          bad_gateway = 502,
          service_unavailable = 503,
          gateway_timeout = 504,
          http_version_not_supported = 505,
          not_extended = 510,
          network_authentication_required = 511
      };

   enum http_method {
      kUnkown,
      kGet,
      kPost,
      kPut,
      kHead,
      kDelete
   };

  inline std::string	getMethodString(http_method m)
  {
    	switch (m) {
        case kGet:
    		  return "GET";
    	  case kPost: 
    		  return "POST";
    	  case  kPut:
          return "PUT";
    	  case kDelete:
          return "DELETE";
    	  case  kHead:
    		  return "HEAD";
    	  default:
    		  return "";
    	}
  }
  inline http_method parseMethod(const Slice & str)
  {
      if(str.starts_with("GET")) {
        return kGet;
      } else if(str.starts_with("POST")){
        return kPost;
      } else if(str.starts_with("PUT")) {
        return kPut;
      } else if(str.starts_with("DELETE")) {
        return kDelete;
      } else if(str.starts_with("HEAD")) {
        return kHead;
      }
      
      return kUnkown;
  }

  inline http_method parseMethod(const std::string & str)
  {
      return parseMethod(Slice(str));  
  }

  enum http_req_parse_status {
    e_request,
    e_header,
    e_body,
    g_all
  };

 enum http_req_packet_status {
    e_request,
    e_header,
    e_body,
    g_all
  };

 enum http_res_parse_status {
    e_stateline,
    e_header,
    e_body,
    g_all
  };

  enum http_res_packet_status {
    e_stateline,
    e_header,
    e_body,
    g_all
  };

  
 inline std::string get_string(http_status_code c) 
 {
          switch (c) {
              case uninitialized:
                  return "Uninitialized";
              case continue_code:
                  return "Continue";
              case switching_protocols:
                  return "Switching Protocols";
              case ok:
                  return "OK";
              case created:
                  return "Created";
              case accepted:
                  return "Accepted";
              case non_authoritative_information:
                  return "Non Authoritative Information";
              case no_content:
                  return "No Content";
              case reset_content:
                  return "Reset Content";
              case partial_content:
                  return "Partial Content";
              case multiple_choices:
                  return "Multiple Choices";
              case moved_permanently:
                  return "Moved Permanently";
              case found:
                  return "Found";
              case see_other:
                  return "See Other";
              case not_modified:
                  return "Not Modified";
              case use_proxy:
                  return "Use Proxy";
              case temporary_redirect:
                  return "Temporary Redirect";
              case bad_request:
                  return "Bad Request";
              case unauthorized:
                  return "Unauthorized";
              case payment_required:
                  return "Payment Required";
              case forbidden:
                  return "Forbidden";
              case not_found:
                  return "Not Found";
              case method_not_allowed:
                  return "Method Not Allowed";
              case not_acceptable:
                  return "Not Acceptable";
              case proxy_authentication_required:
                  return "Proxy Authentication Required";
              case request_timeout:
                  return "Request Timeout";
              case conflict:
                  return "Conflict";
              case gone:
                  return "Gone";
              case length_required:
                  return "Length Required";
              case precondition_failed:
                  return "Precondition Failed";
              case request_entity_too_large:
                  return "Request Entity Too Large";
              case request_uri_too_long:
                  return "Request-URI Too Long";
              case unsupported_media_type:
                  return "Unsupported Media Type";
              case request_range_not_satisfiable:
                  return "Requested Range Not Satisfiable";
              case expectation_failed:
                  return "Expectation Failed";
              case im_a_teapot:
                  return "I'm a teapot";
              case upgrade_required:
                  return "Upgrade Required";
              case precondition_required:
                  return "Precondition Required";
              case too_many_requests:
                  return "Too Many Requests";
              case request_header_fields_too_large:
                  return "Request Header Fields Too Large";
              case internal_server_error:
                  return "Internal Server Error";
              case not_implemented:
                  return "Not Implemented";
              case bad_gateway:
                  return "Bad Gateway";
              case service_unavailable:
                  return "Service Unavailable";
              case gateway_timeout:
                  return "Gateway Timeout";
              case http_version_not_supported:
                  return "HTTP Version Not Supported";
              case not_extended:
                  return "Not Extended";
              case network_authentication_required:
                  return "Network Authentication Required";
              default:
                  return "Unknown";
          }
      }
  }
}

#endif 
