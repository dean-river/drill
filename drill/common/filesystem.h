#ifndef WALLE_FILESYSTEM_H_
#define WALLE_FILESYSTEM_H_

#include <cstdio>
#include <string>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>

using std::string;
using std::vector;

namespace drill {
namespace common {

bool fileExist( const string &file );

bool isLink( const string &file );

bool isDir( const string &file );

bool mkLink( const string &srcfile, const string &destfile );

bool link(const string &srcfile, const string &destfile);

size_t fileSize( const string &file );


time_t fileTime( const string &file );

string filePath( const string &file );


string fileName( const string &file );

bool renameFile( const string &oldname, const string &newname );


bool copyFile( const string &srcfile, const string &destfile );


bool deleteFile( const string &file );


bool moveFile( const string &srcfile, const string &destfile );

vector<string> dirFiles( const string &dir );

bool makeDir( const string &dir, 
	            const mode_t mode = S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH );


bool copyDir( const string &srcdir, const string &destdir );

bool deleteDir( const string &dir );


bool moveDir( const string &srcdir, const string &destdir );
		  
}
}
#endif
