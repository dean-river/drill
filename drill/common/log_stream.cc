#include <drill/common/log_stream.h>

namespace drill{
namespace common {

LogStream& LogStream::operator<<(short v)
{
	*this<<static_cast<int>(v);
	 return *this;
}
LogStream& LogStream::operator<<(unsigned short v)
{		
	*this<<static_cast<unsigned int>(v);
	return *this;
}
LogStream& LogStream::operator<<(int v)
{
	appendInt(v);
	return *this;
}
LogStream& LogStream::operator<<(unsigned int v)
{
	appendInt(v);
	return *this;
}
LogStream& LogStream::operator<<(long v)
{
	appendInt(v);
	return *this;

}
LogStream& LogStream::operator<<(unsigned long v)
{

	appendInt(v);

	return *this;
}
LogStream& LogStream::operator<<(long long v)
{

		appendInt(v);

	return *this;
}
LogStream& LogStream::operator<<(unsigned long long v) 
{
	appendInt(v);
	return *this;
}
//  self& LogStream::operator<<(const Console::TextColor& color);
LogStream& LogStream::operator<<(const void* p)
{
	uintptr_t v = reinterpret_cast<uintptr_t>(p);
	std::string ret =  format_addr_hex(v);
	if (_buffer.avail() > ret.length()+2) {
		_buffer.append("0x",2);
		_buffer.append(ret.c_str(), ret.length());
	}
	return *this;
}

LogStream& LogStream::operator<<(double v)
{

	std::string result = format_double(v);
	*this<<result;
	return *this;
}
	


}
}
