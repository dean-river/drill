#include <drill/common/string.h>
#include <iomanip>
#include <cstring>
#include <cstdio>

namespace drill {
namespace common {

std::string hex_encode(const void *data, size_t size)
{
    const unsigned char* const cdata
        = static_cast<const unsigned char* const>(data);

    std::string out;
    out.resize(size * 2);

    static const char xdigits[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    std::string::iterator oi = out.begin();
    for (const unsigned char* si = cdata; si != cdata + size; ++si)
    {
        *oi++ = xdigits[(*si & 0xF0) >> 4];
        *oi++ = xdigits[(*si & 0x0F)];
    }

    return out;

}

std::string hex_decode(const void *data, size_t size)
{
	  std::string result;
	  result.reserve(size+1);
	  const unsigned char *buff = static_cast<const unsigned char*>(data);
	  const unsigned char *end = static_cast<const unsigned char*>(data) + size;
	  while (*buff > '\0' && *buff <= ' ') {
			  buff++;
	  }
	  
	  while (buff != end) {

	  unsigned char num = 0;
	  unsigned char c = *(buff++);
	  if (c >= '0' && c <= '9') {
		  num = c - '0';
	  } else if (c >= 'a' && c <= 'f') {
		  num = c - 'a' + 10;
	  } else if (c >= 'A' && c <= 'F') {
		  num = c - 'A' + 10;
	  } else if (c == '\0') {
		   break;
	  }
	  c = *(buff++);
	  if (c >= '0' && c <= '9') {
		  num = num * 0x10 + c - '0';
	  } else if (c >= 'a' && c <= 'f') {
		  num = num * 0x10 + c - 'a' + 10;
	  } else if (c >= 'A' && c <= 'F') {
		  num = num * 0x10 + c - 'A' + 10;
	  } else if (c == '\0') {
		  result.append(1, (char)num);
		   break;
	  }
		  result.append(1,(char)num);
	}
	
	
	return result;
	
}

std::string hex_encode(const std::string &s)
{
	return hex_encode(s.data(), s.size());
}
size_t string_split(const std::string& str, char delim, 
					 std::vector<std::string>& elems)
{

  elems.clear();
  std::string::const_iterator it = str.begin();
  std::string::const_iterator pv = it;
  while (it != str.end()) {
    if (*it == delim) {
      std::string col(pv, it);
      elems.push_back(col);
      pv = it + 1;
    }
    ++it;
  }
  std::string col(pv, it);
  elems.push_back(col);
  return elems.size();

}
std::string format_addr_hex(uintptr_t value)
{
        const char digitsHex[] = "0123456789ABCDEF";
        char buf[128];
        uintptr_t i = value;
	    char* p = buf;
	
	    do {
		    int lsd = i % 16;
		    i /= 16;
		    *p++ = digitsHex[lsd];
	    } while (i != 0);
	
	    *p = '\0';
	    std::reverse(buf, p);
	
	  return std::string(buf, p - buf);
}

std::string  format_double(const double &value)
{
	char buff[32];
	snprintf(buff, sizeof(buff)-1, "%.12g",value);
	return std::string(buff);
}
size_t string_split(const std::string& str, const std::string& delims,
			 std::vector<std::string>& elems)
{
  elems.clear();
  std::string::const_iterator it = str.begin();
  std::string::const_iterator pv = it;
  while (it != str.end()) {
    while (delims.find(*it, 0) != std::string::npos) {
      std::string col(pv, it);
      elems.push_back(col);
      pv = it + 1;
      break;
    }
    ++it;
  }
  std::string col(pv, it);
  elems.push_back(col);
  return elems.size();
}

int64_t string_to_int(const std::string &str)
{
	return string_to_int(str.data(), str.size());
}

int64_t string_to_int(const char *buff, size_t len)
{	
	const char *str = buff;
	const char *end = buff+len;
	while (str !=end && *str > '\0' && *str <= ' ') {
    	str++;
  	}
	if(str == end) {
		return 0L;
	}
  	int32_t sign = 1;
  	int64_t num = 0;
	
  	if (*str == '-') {
    	str++;
    	sign = -1;
  	} else if (*str == '+') {
    	str++;
  	}
  	while (str != end && *str != '\0') {
    	if (*str < '0' || *str > '9') 
			break;
   		num = num * 10 + *str - '0';
    	str++;
  }
  return num * sign;
}

int64_t	string_to_inth(const std::string &str)
{
	return string_to_inth(str.data(), str.size());
}
int64_t string_to_inth(const char *buff, size_t len)
{
	const char *str = buff;
	const char *end = buff + len;
	while (str != end && *str > '\0' && *str <= ' ') {
    	str++;
  	}
  	if (str[0] == '0' && (str[1] == 'x' || str[1] == 'X')) {
    	str += 2;
  	}
  	int64_t num = 0;
 	 while (str != end) {
    	if (*str >= '0' && *str <= '9') {
      		num = num * 0x10 + *str - '0';
    	} else if (*str >= 'a' && *str <= 'f') {
      		num = num * 0x10 + *str - 'a' + 10;
    	} else if (*str >= 'A' && *str <= 'F') {
     		 num = num * 0x10 + *str - 'A' + 10;
    	} else {
      		break;
   		}
    	str++;
  	}
  return num;
}


std::string& replace_all(std::string& str, const std::string& needle,
                        const std::string& instead)
{
    std::string::size_type lastpos = 0, thispos;

    while ((thispos = str.find(needle, lastpos)) != std::string::npos)
    {
        str.replace(thispos, needle.size(), instead);
        lastpos = thispos + instead.size();
    }
    return str;

}
std::string& trim(std::string& str, const std::string& drop) {
    std::string::size_type pos = str.find_last_not_of(drop);
    if (pos != std::string::npos) {
        str.erase(pos + 1);
        pos = str.find_first_not_of(drop);
        if (pos != std::string::npos) str.erase(0, pos);
    }
    else
        str.erase(str.begin(), str.end());

    return str;
}

void toupper(std::string &str)
{
	std::string::iterator oi = str.begin();
	while(oi != str.end()) {
		if(*oi <= 'z' && *oi >= 'a') {
			*oi -= 32;
		}
		++oi;
	}
}
void tolower(std::string &str)
{
	std::string::iterator oi = str.begin();
	while(oi != str.end()) {
		if(*oi <= 'Z' && *oi >= 'A') {
			*oi += 32;
		}
		++oi;
	}

}
bool parse_size(const char* str, uint64_t& size, char default_unit)
{
    char* endptr;
    size = strtoul(str, &endptr, 10);
    if (!endptr) return false;                    // parse failed, no number

    while (endptr[0] == ' ') ++endptr;            // skip over spaces

    // multiply with base ^ power
    unsigned int base = 1000;
    unsigned int power = 0;

    if (endptr[0] == 'k' || endptr[0] == 'K')
        power = 1, ++endptr;
    else if (endptr[0] == 'm' || endptr[0] == 'M')
        power = 2, ++endptr;
    else if (endptr[0] == 'g' || endptr[0] == 'G')
        power = 3, ++endptr;
    else if (endptr[0] == 't' || endptr[0] == 'T')
        power = 4, ++endptr;
    else if (endptr[0] == 'p' || endptr[0] == 'P')
        power = 5, ++endptr;

    // switch to power of two (only if power was set above)
    if ((endptr[0] == 'i' || endptr[0] == 'I') && power != 0)
        base = 1024, ++endptr;

    // byte indicator
    if (endptr[0] == 'b' || endptr[0] == 'B') {
        ++endptr;
    }
    else if (power == 0)
    {
        // no explicit power indicator, and no 'b' or 'B' -> apply default unit
        switch (default_unit)
        {
        default: break;
        case 'k': power = 1, base = 1000;
            break;
        case 'm': power = 2, base = 1000;
            break;
        case 'g': power = 3, base = 1000;
            break;
        case 't': power = 4, base = 1000;
            break;
        case 'p': power = 5, base = 1000;
            break;
        case 'K': power = 1, base = 1024;
            break;
        case 'M': power = 2, base = 1024;
            break;
        case 'G': power = 3, base = 1024;
            break;
        case 'T': power = 4, base = 1024;
            break;
        case 'P': power = 5, base = 1024;
            break;
        }
    }

    // skip over spaces
    while (endptr[0] == ' ') ++endptr;

    // multiply size
    for (unsigned int p = 0; p < power; ++p)
        size *= base;

    return (endptr[0] == 0);

}
std::string format_si_size(uint64_t number)
{
    // may not overflow, std::numeric_limits<uint64_t>::max() == 16 EiB
    double multiplier = 1000.0;
    static const char* SI_endings[] = {
        "", "k", "M", "G", "T", "P", "E"
    };
    unsigned int scale = 0;
    double number_d = static_cast<double>(number);
    while (number_d >= multiplier) {
        number_d /= multiplier;
        ++scale;
    }
    std::ostringstream out;
    out << std::fixed << std::setprecision(3) << number_d
        << ' ' << SI_endings[scale];
    return out.str();

}
std::string format_lec_size(uint64_t number)
{
		// may not overflow, std::numeric_limits<uint64_t>::max() == 16 EiB
		double multiplier = 1024.0;
		static const char* IEC_endings[] = {
			"", "Ki", "Mi", "Gi", "Ti", "Pi", "Ei"
		};
		unsigned int scale = 0;
		double number_d = static_cast<double>(number);
		while (number_d >= multiplier) {
			number_d /= multiplier;
			++scale;
		}
		std::ostringstream out;
		out << std::fixed << std::setprecision(3) << number_d
			<< ' ' << IEC_endings[scale];
		return out.str();
}


}
}
