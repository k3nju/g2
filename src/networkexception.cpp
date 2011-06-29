#include "networkexception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	NetworkException::NetworkException( string message, int number, int code )
		:Exception( message, number, code )
		{
		}

	//-----------------------------------------------------------------------------------------//
	NetworkException::NetworkException( const NetworkException &rhs )
		:Exception( rhs )
		{
		}

	//-----------------------------------------------------------------------------------------//
	NetworkException& NetworkException::operator = ( const NetworkException &rhs )
		{
		Exception::operator = ( rhs );
		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	NetworkException::~NetworkException() throw()
		{
		}
	}
