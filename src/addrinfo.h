#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "uncopyable.h"

namespace g2
	{
	class AddrInfo :private g2::Uncopyable< AddrInfo >
		{
		public:
			enum cs_type
				{
				CLIENT = 0,				
				SERVER = AI_PASSIVE
				};
			
			AddrInfo(
				// AF_UNSPEC or AF_INET or AF_INET6
				int family,
				// SOCK_STREAM or SOCK_DGRAM
				int socketType,
				 // for client or for server
				enum cs_type csType,
				// ip address or hostname or NULL
				const char *node,
				// service name or port number or NULL
				const char *service );
			AddrInfo();
			
			~AddrInfo();

			bool Set( int family, int socketType, enum cs_type csType, const char *node, const char *service );
			bool Set( int family, int socketType, enum cs_type csType, const char *node, const char *service, int &resultNum );
			const struct addrinfo* Get() const;
			
			bool IsSame( struct sockaddr &skad, socklen_t len );

		private:
			struct addrinfo* GetAddrInfo( int family, int socketType, enum cs_type csType, const char *node, const char *service, int &resultNum );
			struct addrinfo *result_;
		};	
	}  


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__AddrInfoTest

using namespace g2;

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/addrinfo.h\
$(here)/addrinfo.cpp\
$(here)/nameinfo.h\
$(here)/nameinfo.cpp
</Makefile.am>
*/

#include <stdio.h>
#include "nameinfo.h"
#include "networkexception.h"

void Dump( AddrInfo &ai )
	{
	const struct addrinfo *res = ai.Get();

	for( const struct addrinfo *cur = res;
		 cur != NULL;
		 cur = cur->ai_next )
		{

		printf( "ai_family:" );
		switch( cur->ai_family )
			{
			case AF_INET:
				puts( "AF_INET" );
				break;
			case AF_INET6:
				puts( "AF_INET6" );
				break;
			default:
				printf( "%d\n", cur->ai_family );
				break;
			};

		printf( "ai_socktype:" );
		switch( cur->ai_socktype )
			{
			case SOCK_STREAM:
				puts( "SOCK_STREAM" );
				break;
			case SOCK_DGRAM:
				puts( "SOCK_DGRAM" );
				break;
			default:
				printf( "%d\n", cur->ai_socktype );
				break;
			};

		printf( "protocol:%d\n", cur->ai_protocol );

		NameInfo ni( cur->ai_addr, cur->ai_addrlen );
		printf( "ip:%s\n", ni.GetIPAddress().c_str() );
		printf( "port:%s\n", ni.GetService().c_str() );

		puts( " ---------------" );		
		}
	}

#define TRY_CATCH( code ) \
	try{ code }catch( NetworkException &E ){ E.Output(); }

int GutsEntryAddrInfoTest( int, char** )
	{
	printf( "%d %d %d\n",sizeof( struct sockaddr ), sizeof( struct sockaddr_in ), sizeof( sockaddr_storage ) );
	
	// == server interface tests ==
	puts( "////////////////////// SERVER TEST ");
	
	// INADDR_ANY
	puts( "### INADDR_ANY" );
	TRY_CATCH(
		AddrInfo ai( AF_UNSPEC, SOCK_STREAM, AddrInfo::SERVER, NULL, "56000" );
		Dump( ai );
		);

	// localhost
	puts( "### localhost" );
	TRY_CATCH(
		AddrInfo ai( AF_INET, SOCK_STREAM, AddrInfo::SERVER, "localhost", "56000" );
		Dump( ai );
		);

	// 192.168.11.2
	puts( "### 192.168.11.2" );
	TRY_CATCH(
		AddrInfo ai( AF_INET, SOCK_STREAM, AddrInfo::SERVER, "192.168.11.2", "56000" );
		Dump( ai );
		);

	// == client interface test ==
	puts( "////////////////////// CLIENT TEST ");

	// localhost
	puts( "### localhost" );
	TRY_CATCH(
		AddrInfo ai( AF_UNSPEC, SOCK_STREAM, AddrInfo::CLIENT, "localhost", "56000" );
		Dump( ai );
		);	

	// 192.168.11.2
	puts( "## 192.168.11.2" );
	TRY_CATCH(
		AddrInfo ai( AF_INET, SOCK_STREAM, AddrInfo::CLIENT, "192.168.11.2", "56000" );
		Dump( ai );
		);		

	// www.microsoft.com
	puts( "## www.microsoft.com" );
	TRY_CATCH(
		AddrInfo ai( AF_INET, SOCK_STREAM, AddrInfo::CLIENT, "www.microsoft.com", "56000" );
		Dump( ai );
		);


	TRY_CATCH(
		AddrInfo ai;
		printf( "set result = %d\n", ai.Set( AF_INET, SOCK_STREAM, AddrInfo::CLIENT, "localhsot", "56000" ) );
		printf( "addrinfo = %x\n", ai.Get() ); // must be NULL 
		printf( "set result = %d\n", ai.Set( AF_INET, SOCK_STREAM, AddrInfo::CLIENT, "www.microsoft.com", "56000" ) );
		printf( "addrinfo = %x\n", ai.Get() ); // must not be NULL 
		printf( "set result = %d\n", ai.Set( AF_INET, SOCK_STREAM, AddrInfo::CLIENT, "localhsot", "56000" ) );
		printf( "addrinfo = %x\n", ai.Get() ); // must NOT be NULL. ai must keep valid state for exception-safe
		);	

	
	
	return 0;
	}

#endif
#endif
