#include <iostream>
#include <fstream>
#include <cstdlib>
#include <drill/common/properties.h>
#include <drill/common/string.h>

using namespace drill::common;

Properties::Properties()
{
    propmap = new std::map<std::string, std::string>;
}

Properties::Properties(const Properties &p)
{
    propmap = new std::map<std::string, std::string>(*(p.propmap));
}

Properties::~Properties()
{
    delete propmap;
}

int Properties::loadProperties(const char* fileName, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::ifstream input(fileName);
    std::string line;

    if(!input.is_open()) 
    {
        std::cout<<  "Properties::loadProperties() Could not open the file:" << fileName <<std::endl;
        return(-1);
    }
    loadProperties(input, delimiter, verbose, multiline);
    input.close();
    return 0;
}

int Properties::loadProperties(std::istream &ist, char delimiter, bool verbose, bool multiline /*=false*/)
{
    std::string line;

    while(ist)
    {
        getline(ist, line);                       //read one line at a time
        if  (line.find('#') == 0)
            continue;                               //ignore comments
        std::string::size_type pos =
            line.find(delimiter);                   //find the delimiter
        
        if( pos == line.npos )
            continue;                               //ignore if no delimiter is found
        std::string key = line.substr(0,pos);       // get the key
        key = trim(key);
        std::string value = line.substr(pos+1);     //get the value
        value = trim(value);

		if (multiline)
        	(*propmap)[key] += value;					// allow properties to be spread across multiple lines
        else
        	(*propmap)[key] = value;
        
        if( verbose)
            std::cout << "Loading key " << key  << " with value " << (*propmap)[key] << std::endl ;
    }
    return 0;
}



void Properties::setValue(const std::string key, const std::string value) {
    (*propmap)[key] = value;
    return;
}

std::string Properties::getValue(std::string key, std::string def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (*propmap)[key];
}
const char* Properties::getValue(std::string key, const char* def) const
{
    if(propmap->find(key) == propmap->end()) return def;
        return (((*propmap)[key]).c_str());
}

int Properties::getValue(std::string key, int def)
{
	int ret = def;
    if(propmap->find(key) == propmap->end())  {
    	return def;
    }

    bool bret = from_str(((*propmap)[key]), ret);
        return bret ? ret : def;
}
long Properties::getValue(std::string key, long def)
{
	long ret = def;
    if(propmap->find(key) == propmap->end())  {
    	return def;
    }

    bool bret = from_str(((*propmap)[key]), ret);
        return bret ? ret : def;
}

long long Properties::getValue(std::string key, long long def)
{
	long long ret = def;
    if(propmap->find(key) == propmap->end())  {
    	return def;
    }

    bool bret = from_str(((*propmap)[key]), ret);
        return bret ? ret : def;
}

uint64_t Properties::getValue(std::string key, uint64_t def)
{
	uint64_t ret = def;
    if(propmap->find(key) == propmap->end())  {
    	return def;
    }

    bool bret = from_str(((*propmap)[key]), ret);
        return bret ? ret : def;
}

double Properties::getValue(std::string key, double def)
{
	double ret = def;
    if(propmap->find(key) == propmap->end())  {
    	return def;
    }

    bool bret = from_str(((*propmap)[key]), ret);
        return bret ? ret : def;
}

void Properties::getList(std::string &outBuf, std::string linePrefix) const {
  std::map<std::string, std::string>::iterator iter;

  for (iter = propmap->begin(); iter != propmap->end(); iter++) {
    if ((*iter).first.size() > 0) {
      outBuf += linePrefix;
      outBuf += (*iter).first;
      outBuf += '=';
      outBuf += (*iter).second;
      outBuf += '\n';
    }
  }

  return;
}
