#include <drill/httpxx/http_url.h>
#include <iostream>

namespace drill {
namespace httpxx {

    Url::Url ( const std::string& url, bool isConnect )
        : _data(url)
    {
        const int result = ::http_parser_parse_url(_data.data(), _data.size(), isConnect ? 1 : 0, &_fields);
        if (result != 0)
        { 
        	_valid = false;
           // TODO: handle failure.
        }
		_valid = true;
    }

    Url::Url ( const char * data, std::size_t size, bool isConnect )
        : _data(data, size)
    {
        const int result = ::http_parser_parse_url
            (_data.data(), _data.size(), isConnect ? 1 : 0, &_fields);
        if (result != 0)
        {
        	_valid = false;
            // TODO: handle failure.
        }
		_valid = true;
    }
	bool Url::valid() const
	{
		return _valid;
	}
    bool Url::hasSchema () const
    {
        return (hasField(UF_SCHEMA));
    }

    std::string Url::schema () const
    {
        return (getField(UF_SCHEMA));
    }

    bool Url::hasHost () const
    {
        return (hasField(UF_HOST));
    }

    std::string Url::host () const
    {
        return (getField(UF_HOST));
    }

    bool Url::hasPort () const
    {
        return (hasField(UF_PORT));
    }

    std::string Url::port () const
    {
        return (getField(UF_PORT));
    }

    bool Url::hasPath () const
    {
        return (hasField(UF_PATH));
    }

    std::string Url::path () const
    {
        return (getField(UF_PATH));
    }

    bool Url::hasQuery () const
    {
        return (hasField(UF_QUERY));
    }

    std::string Url::query () const
    {
        return (getField(UF_QUERY));
    }

    bool Url::hasFragment () const
    {
        return (hasField(UF_FRAGMENT));
    }

    std::string Url::fragment () const
    {
        return (getField(UF_FRAGMENT));
    }

    bool Url::hasField ( http_parser_url_fields field ) const
    {
        return ((_fields.field_set & (1 << int(field))) != 0);
    }

    std::string Url::getField ( http_parser_url_fields field ) const
    {
        if (!hasField(field)) {
            return ("");
        }
        return (_data.substr(_fields.field_data[int(field)].off,
                              _fields.field_data[int(field)].len));
    }

	std::string Url::absolutePath() const
	{
		std::string absolute_path;
		if(hasPath())
		{
			absolute_path += path();
		}

		if(absolute_path.empty())
		{
			absolute_path = "/";
		}
		
		return absolute_path;
	}

	std::string Url::pathAndQuery() const
	{
		std::string path_and_query(absolutePath());

		if(hasQuery())
		{
			path_and_query += "?";
			path_and_query += query();
		}

		return path_and_query;
	}

}
}
