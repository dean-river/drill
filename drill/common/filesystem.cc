#include <drill/common/filesystem.h>
#include <dirent.h>
#include <unistd.h>
using std::string;
using std::vector;

namespace drill {
namespace common {

bool fileExist( const string &file ) {
	if ( ::access(file.c_str(),F_OK) == 0 )
		return true;
	else
		return false;
}

bool isLink( const string &file ) {
	struct stat statbuf;
	
	if( ::lstat(file.c_str(),&statbuf) == 0 ) {
		if ( S_ISLNK(statbuf.st_mode) != 0 )
			return true;
	}
	return false;
}

bool isDir( const string &file ) {
	struct stat statbuf;
	
	if( ::stat(file.c_str(),&statbuf) == 0 ) {
		if ( S_ISDIR(statbuf.st_mode) != 0 )
			return true;
	}
	return false;
}


bool mkLink( const string &srcfile, const string &destfile ) {
	if ( ::symlink(srcfile.c_str(),destfile.c_str()) == 0 )
		return true;
	else
		return false;
}
bool link(const string &srcfile, const string &destfile)
{
	if ( ::link(srcfile.c_str(),destfile.c_str()) == 0 )
		return true;
	else
		return false;
}

size_t fileSize( const string &file ) {
	struct stat statbuf;
	
	if( stat(file.c_str(),&statbuf)==0 )
		return statbuf.st_size;
	else		
		return -1;
}

time_t fileTime( const string &file ) {
	struct stat statbuf;
	
	if( stat(file.c_str(),&statbuf)==0 )
		return statbuf.st_mtime;
	else		
		return -1;
}

string filePath( const string &file ) {
	size_t p;
	if ( (p=file.rfind("/")) != file.npos )
		return file.substr( 0, p );
	return string( "" );
}

string fileName( const string &file ) {
	size_t p;
	if ( (p=file.rfind("/")) != file.npos )
		return file.substr( p+1 );
	return file;
}

bool renameFile( const string &oldname, const string &newname ) {
	if ( ::rename(oldname.c_str(),newname.c_str()) != -1 )
		return true;
	else
		return false;
}


bool copyFile( const string &srcfile, const string &destfile ) {
	FILE *src=NULL, *dest=NULL;
	if ( (src=fopen(srcfile.c_str(),"rb")) == NULL ) {
		return false;
	}
	if ( (dest=fopen(destfile.c_str(),"wb+")) == NULL ) {
		fclose( src );
		return false;
	}
	
	const int bufsize = 8192;
	char buf[bufsize];
	size_t n;
	while ( (n=fread(buf,1,bufsize,src)) >= 1 ) {
		if ( fwrite(buf,1,n,dest) != n ) {
			fclose( src );
			fclose( dest );
			return false;
		}
	}
	
	fclose( src );
	fclose( dest );

	//chmod to 0666
	mode_t mask = umask( 0 );
	chmod( destfile.c_str(), S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH );
	umask( mask );
	
	return true;
}

bool deleteFile( const string &file ) {
	if ( ::remove(file.c_str()) == 0 )
		return true;
	else
		return false;
}

bool moveFile( const string &srcfile, const string &destfile ) {
	if ( renameFile(srcfile,destfile) )
		return true;

	// rename fail, copy and delete file		
	if ( copyFile(srcfile,destfile) ) {
		if ( deleteFile(srcfile) )
			return true;
	}
	
	return false;
}

vector<string> dirFiles( const string &dir ) {
	vector<string> files;
	string file;
	DIR *pdir = NULL;
	dirent *pdirent = NULL;
	
	if ( (pdir = ::opendir(dir.c_str())) != NULL ) {
		while ( (pdirent=readdir(pdir)) != NULL ) {
			file = pdirent->d_name;
			if ( file!="." && file!=".." ) {
				if ( isDir(dir+"/"+file) ) 
					file = "/"+file;
				files.push_back( file );
			}
		}
		::closedir( pdir );
	}
	
	return files;
}

bool makeDir( const string &dir, const mode_t mode ) {
	// check
	size_t len = dir.length();
	if ( len <= 0 )	return false;

	string tomake;
	char curchr;
	for( size_t i=0; i<len; ++i ) {
		// append
		curchr = dir[i];
		tomake += curchr;
		if ( curchr=='/' || i==(len-1) ) {
			// need to mkdir
			if ( !fileExist(tomake) && !isDir(tomake) ) {
				// able to mkdir
				mode_t mask = umask( 0 );				
				if ( mkdir(tomake.c_str(),mode) == -1 ) {
					umask( mask );
					return false;
				}
				umask( mask );
			}
		}
	}
	
	return true;
}	

bool copyDir( const string &srcdir, const string &destdir ) {	
	vector<string> files = dirFiles( srcdir );
	string from;
	string to;
	
	if ( !fileExist(destdir) )
		makeDir( destdir );
	
	for ( size_t i=0; i<files.size(); ++i ) {
		from = srcdir + "/" + files[i];
		to = destdir + "/" + files[i];
		
		if ( files[i][0] == '/' ) {
			 if ( !copyDir(from,to) )
				return false;
		}
		
		else if ( !copyFile(from,to) )
			return false;
	}
	
	return true;
}


bool deleteDir( const string &dir ) {	
	vector<string> files = dirFiles( dir );
	string todel;
	
	// 删除文件
	for ( size_t i=0; i<files.size(); ++i ) {
		todel = dir + "/" + files[i];
		
		// 子目录,递归调用
		if ( files[i][0] == '/' ) {
			 if ( !deleteDir(todel) )
				return false;
		}
		
		// 文件
		else if ( !deleteFile(todel) )
			return false;
	}
	
	// 删除目录
	if ( rmdir(dir.c_str()) == 0 )
		return true;

	return false;
}

bool moveDir( const string &srcdir, const string &destdir ) {
	if ( renameFile(srcdir,destdir) )
		return true;

	// rename fail, copy and delete dir	
	if ( copyDir(srcdir,destdir) ) {
		if (deleteDir(srcdir) )
			return true;
	}

	return false;
}

bool readFileToString(const std::string & path, std::string *content)
{
	//fixme:std::forward
	if(!fileExist(path)) {
		return false;
	}
	
	size_t filesize = fileSize(path);

	content->clear();
	if(filesize == 0 ) {
		
		return true;
	}

	content->reserve(filesize);

	int fd = ::open(path.c_str(),O_RDONLY|O_CLOEXEC);

	if(fd == -1) {
		::close(fd);
		return false;
	}

	ssize_t n = ::read(fd,&(*content)[0],filesize);
	::close(fd);
	if(n <= 0 || static_cast<size_t>(n) != filesize) {	
		return false;
	}

	content->resize(n);
	return true;
	
}

bool writeStringToFile(const std::string &path, const char *content, size_t len)
{
	if(!content) {
		return false;
	}



	int fd = ::open(path.c_str(),O_WRONLY|O_CLOEXEC|O_CREAT|O_TRUNC, 0644);

	if(fd == -1) {
		::close(fd);
		return false;
	}

			//just create a file
	if(len == 0) {
		::close(fd);
		return true;
	}

	ssize_t n = ::write(fd,content,len);
	::close(fd);
	if(n <= 0 || static_cast<size_t>(n) != len) {	
		return false;
	}
	
	return true;


}

}
} // namespace

