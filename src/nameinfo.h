#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string>
#include "uncopyable.h"

namespace g2
	{
	class NameInfo :private g2::Uncopyable
		{
		public:
			NameInfo( const struct sockaddr *sa, socklen_t salen );

			const std::string GetIPAddress() const { return host_; };
			const std::string GetService() const { return service_; };

		private:
			std::string host_;
			std::string service_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__NameInfoTest

using namespace g2;

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/addrinfo.h\
$(here)/addrinfo.cpp\
$(here)/nameinfo.h\
$(here)/nameinfo.cpp\
</Makefile.am>
*/

#include <stdio.h>
#include "networkexception.h"
#include "addrinfo.h"
#include "netutility.h"
#include "debugutility.h"

void Dump( NameInfo &ni )
	{
	printf( "%s:%s\n",
			ni.GetIPAddress().c_str(),
			ni.GetService().c_str() );
	}

#define TRY_CATCH( code ) \
	try{ code }catch( NetworkException &E ){ E.Output(); }

int GutsEntryNameInfoTest(int,char**)
	{
	struct sockaddr_in sai;
	memset( &sai, 0, sizeof( sai ) );
	
	NetUtil::SetSockAddrIn( &sai, "localhost", 56000 );
	TRY_CATCH(
		NameInfo ni( (struct sockaddr*)&sai, sizeof( sai ) );
		Dump( ni );
		);

	NetUtil::SetSockAddrIn( &sai, "www.microsoft.com", 80 );
	TRY_CATCH(
		NameInfo ni( (struct sockaddr*)&sai, sizeof( sai ) );
		Dump( ni );		
		);
	
	return 0;
	}


#endif
#endif
