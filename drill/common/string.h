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

template <typename Type>
static inline
std::string to_str(const Type& t) {
    std::ostringstream oss;
    oss << t;
    return oss.str();
}

template <typename Type>
static inline
bool from_str(const std::string& str, Type& outval) {
    std::istringstream is(str);
    is >> outval;
    return is.eof();
}

std::vector<std::string> split(
    const std::string& str, char sep,
    std::string::size_type limit = std::string::npos);

std::vector<std::string> split(
    const std::string& str, const std::string& sepstr,
    std::string::size_type limit = std::string::npos);


std::vector<std::string> split(const std::string& str, const std::string& sep,
      unsigned int min_fields,
      unsigned int limit_fields = std::numeric_limits<unsigned int>::max());

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


}//common end
}//drill end
#endif
