#include "addrinfo.h"
#include <string.h>
#include <cassert>
#include "networkexception.h"


namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	AddrInfo::AddrInfo( int family,
						int socketType,
						enum cs_type csType,
						const char *node,
						const char *service )
		:result_( NULL )
		{
		int ret = 0;
		result_ = GetAddrInfo( family, socketType, csType, node, service, ret );
		if( result_ == NULL )
			{
			string msg = "getaddrinfo() failed. " + string( gai_strerror( ret ) );
			throw NetworkException( msg, ret, NAME_RESOLUTION_FAILURE );
			}
		}

	//-----------------------------------------------------------------------------------------//
	AddrInfo::AddrInfo():result_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	AddrInfo::~AddrInfo()
		{
		if( result_ != NULL )
			{
			freeaddrinfo( result_ );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool AddrInfo::Set( int family, int socketType, enum cs_type csType, const char *node, const char *service )
		{
		int ret = 0;
		struct addrinfo *tmp = GetAddrInfo( family, socketType, csType, node, service, ret );
		if( tmp == NULL )
			{
			return false;
			}

		if( result_ != NULL )
			{
			freeaddrinfo( result_ );
			}

		result_ = tmp;

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	bool AddrInfo::Set( int family, int socketType, enum cs_type csType, const char *node, const char *service, int &resultNum )
		{
		struct addrinfo *tmp = GetAddrInfo( family, socketType, csType, node, service, resultNum );
		if( tmp == NULL )
			{
			return false;
			}

		if( result_ != NULL )
			{
			freeaddrinfo( result_ );
			}

		result_ = tmp;

		return true;
		}	

	//-----------------------------------------------------------------------------------------//
	const struct addrinfo* AddrInfo::Get() const
		{
		return result_;
		}

	//-----------------------------------------------------------------------------------------//
	struct addrinfo* AddrInfo::GetAddrInfo( int family, int socketType, enum cs_type csType, const char *node, const char *service, int &resultNum )
		{
		struct addrinfo hints;
		memset( &hints, 0, sizeof( hints ) );

		hints.ai_family = family;
		hints.ai_socktype = socketType;
		hints.ai_flags = (int)csType;
		hints.ai_protocol = 0;
		hints.ai_canonname = NULL;
		hints.ai_addr = NULL;
		hints.ai_next = NULL;

		struct addrinfo *result = NULL;
		
		while( ( resultNum = getaddrinfo( node, service, &hints, &result ) ) == EAI_AGAIN ){}

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	bool AddrInfo::IsSame( struct sockaddr &skad, socklen_t len )
		{
		assert( result_ != NULL && "resolve address before call AddrInfo::IsSame" );
		struct addrinfo *rp = NULL;
		for( rp = result_; rp != NULL; rp = rp->ai_next )
			{
			if( len == rp->ai_addrlen && memcmp( &skad, rp->ai_addr, len ) )
				{
				return true;
				}
			}

		return false;
		}
	}

