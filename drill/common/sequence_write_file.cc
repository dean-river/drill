#include <drill/common/sequence_write_file.h>
#include <cassert>

namespace drill {
namespace common {
	SequenceWriteFile::SequenceWriteFile(const char* filename):
		_fp(::fopen(filename, "ae")),_writesize(0)
	{
		assert(_fp);
		::setbuffer(_fp, _buffer, sizeof _buffer);	
	}
	SequenceWriteFile::SequenceWriteFile(const string& filename):
		_fp(::fopen(filename.c_str(), "ae")),_writesize(0)
	{
		assert(_fp);
		::setbuffer(_fp, _buffer, sizeof _buffer);
	}
	SequenceWriteFile::~SequenceWriteFile()
	{
		if(_fp) {
			::fclose(_fp);
		}
	}
	void SequenceWriteFile::flush()
	{
		if(_fp) {
			::fflush(_fp);
		}
	}
	bool SequenceWriteFile::append(const char* content, const size_t len)
	{
		size_t n = unlockWrite(content, len);
		size_t remain = len -n;
		while (remain > 0) {
			n = unlockWrite(content+len-remain,remain);
			if (n == 0) {
				int err = ferror(_fp);
				if(err) {
					fprintf(stderr,  "SequenceWriteFile::append() failed %d\n", err);
					return false;
				}
			}
			remain = len -n;
		}
		_writesize += len;
		return true;

	}
	size_t SequenceWriteFile::writeSize()
	{
		return _writesize;
	}
	size_t SequenceWriteFile::unlockWrite(const char* content, const size_t len)
	{
		return ::fwrite_unlocked(content, 1, len, _fp);
	}

}
}
