#ifndef DRILL_COMMON_STRING_H_
#define DRILL_COMMON_STINEG_H_

#include <string>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <sstream>
#include <vector>
#include <limits>

namespace drill {

namespace common {

std::string hex_encode(const void *data, size_t size);

std::string hex_decode(const void *data, size_t size);

template <typename T>
std::string hex_encode(const T & obj)
{
	return hex_encode(&obj, sizeof(obj));
}

std::string hex_encode(const std::string &s);

static inline bool start_with(const std::string &str, const std::string &match)
{
	if (match.size() > str.size()) return false;
    return std::equal(match.begin(), match.end(), str.begin());
}

static inline
bool end_with(const std::string& str, const std::string& match)
{
    if (match.size() > str.size()) return false;
    return std::equal(match.begin(), match.end(),
                      str.end() - match.size());
}

int64_t string_to_int(const std::string &str);

int64_t string_to_int(const char *buff, size_t len);
int64_t	string_to_inth(const std::string &str);
int64_t string_to_inth(const char *buff, size_t len);


template <typename T>
static inline
std::string to_str(const T& value) {
	  const char digits[] = "9876543210123456789";
	  const char* zero = digits + 9;
	  char buf[128];
	  T i = value;
	  char* p = buf;
	
	do {
	  int lsd = static_cast<int>(i % 10);
	  i /= 10;
	  *p++ = zero[lsd];
	} while (i != 0);
	
	if (value < 0) {
	  *p++ = '-';
	}
	*p = '\0';
	std::reverse(buf, p);
	
	return std::string(buf, p - buf);

}

template <typename Type>
static inline
bool from_str(const std::string& str, Type& outval) {
    std::istringstream is(str);
    is >> outval;
    return is.eof();
}

size_t string_split(const std::string& str, char delim, std::vector<std::string>& elems);

size_t string_split(const std::string& str, const std::string& delims,
			  std::vector<std::string>& elems);


template <typename Iterator, typename Glue>
static inline
std::string join(const Glue& glue, Iterator first, Iterator last) {
    std::ostringstream oss;
    if (first == last) return oss.str();

    oss << *first++;

    while (first != last) {
        oss << glue;
        oss << *first++;
    }

    return oss.str();
}

template <typename Container, typename Glue>
static inline
std::string join(const Glue& glue, const Container& parts) {
    return join(glue, parts.begin(), parts.end());
}

template <typename T>
static std::string vec_to_string(const std::vector<T>& data) {
    std::ostringstream oss;
    oss << '[';
    for (typename std::vector<T>::const_iterator it = data.begin();
         it != data.end(); ++it)
    {
        if (it != data.begin()) oss << ',';
        oss << *it;
    }
    oss << ']';
    return oss.str();
}

std::string& replace_all(std::string& str, const std::string& needle,
                        const std::string& instead);

std::string& trim(std::string& str, const std::string& drop = " \r\n\t");



/*
* chinese charactor caution, do not deal it
*/
void toupper(std::string &str);
void tolower(std::string &str);

bool parse_size(const char* str, uint64_t& size, char default_unit = 0);
/**************************************
*basement 1000
*
**************************************/
std::string format_si_size(uint64_t number);
/********************************
*basement 1024
**********************************/
std::string format_lec_size(uint64_t number);

std::string format_addr_hex(uintptr_t value);
std::string  format_double(const double &value);

}//common end
}//drill end
#endif
