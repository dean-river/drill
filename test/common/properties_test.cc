#include <string>
#include <iostream>
#include <drill/common/properties.h>

using namespace drill::common;
int main()
{
	Properties conf;
	
	conf.loadProperties("./test.conf",'=',true);
	std::string strout;
	conf.getList(strout,"*");
	
	std::cout<<strout<<std::endl;
	
}
