// Copyright(c) Andre Caron <andre.l.caron@gmail.com>, 2011
//
// This document is covered by the an Open Source Initiative approved license. A
// copy of the license should have been provided alongside this software package
// (see "LICENSE.txt"). If not, terms of the license are available online at
// "http://www.opensource.org/licenses/mit".

#include <drill/httpxx/http_flags.h>
namespace drill {
namespace httpxx {
	Flags::Flags() : _value(Flags::Value(0))
	{

	}

	Flags::Flags(const Flags& flags) : _value(flags._value)
	{

	}

	Flags::Flags(Value value) : _value(value)
	{

	}

	void Flags::setChunked(bool ischunked)
	{
		if(ischunked)
		{
			_value = (Flags::Value)((int)_value | F_CHUNKED);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_CHUNKED) ;
		}
	}

	void Flags::setKeepalive(bool iskeepalive)
	{
		if(iskeepalive)
		{
			_value = (Flags::Value)((int)_value | F_CONNECTION_KEEP_ALIVE);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_CONNECTION_KEEP_ALIVE) ;
		}
	}

	void Flags::setClose(bool isclose)
	{
		if(isclose)
		{
			_value = (Flags::Value)((int)_value | F_CONNECTION_CLOSE);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_CONNECTION_CLOSE) ;
		}
	}

	void Flags::setTrailing(bool istrailing)
	{
		if(istrailing)
		{
			_value = (Flags::Value)((int)_value | F_TRAILING);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_TRAILING) ;
		}
	}

	void Flags::setUpgrade(bool isupgrade)
	{
		if(isupgrade)
		{
			_value = (Flags::Value)((int)_value | F_UPGRADE);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_UPGRADE) ;
		}
	}

	void Flags::setSkipbody(bool isskipbody)
	{
		if(isskipbody)
		{
			_value = (Flags::Value)((int)_value | F_SKIPBODY);
		}
		else
		{
			_value = (Flags::Value)((int)_value | ~F_SKIPBODY) ;
		}
	}

    const Flags Flags::of ( const ::http_parser& parser )
    {
        return (static_cast< ::flags >(parser.flags));
    }

    const Flags Flags::chunked ()
    {
        return (F_CHUNKED);
    }

    const Flags Flags::keepalive ()
    {
        return (F_CONNECTION_KEEP_ALIVE);
    }

    const Flags Flags::close ()
    {
        return (F_CONNECTION_CLOSE);
    }

    const Flags Flags::trailing ()
    {
        return (F_TRAILING);
    }

    const Flags Flags::upgrade ()
    {
        return (F_UPGRADE);
    }

    const Flags Flags::skipbody ()
    {
        return (F_SKIPBODY);
    }
	    
    bool Flags::operator& ( const Flags& rhs ) const
    {
        return ((_value & rhs._value) != 0);
    }

	Flags Flags::operator| ( const Flags& rhs ) const
	{
		return Flags((Value)(_value | rhs._value));
	}

	Flags Flags::operator~ () const
	{
		return Flags((Value)(_value));
	}

}
}
