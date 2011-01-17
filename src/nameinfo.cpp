#include "nameinfo.h"
#include "networkexception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	NameInfo::NameInfo( const struct sockaddr *sa, socklen_t salen )
		{
		char host[ NI_MAXHOST + 1 ] = "";
		char serv[ NI_MAXSERV + 1 ] = "";
		
		int ret = 0;
		while( ( ret = getnameinfo( sa,
									salen,
									host,
									sizeof( host ) - 1,
									serv,
									sizeof( serv ) - 1,
									NI_NUMERICHOST | NI_NUMERICSERV ) ) == EAI_AGAIN ){}

		if( ret != 0 )
			{
			string msg = "getnameinfo() failed. "  + string( gai_strerror( ret ) );
			throw NetworkException( msg, ret, NAME_RESOLUTION_FAILURE );
			}

		host_ = string( host );
		service_ = string( serv );
		}
	}
