#pragma once

#include "addrinfo.h"
#include "socketbase.h"

namespace g2
	{
	class UDPSocket :public g2::SocketBase< UDPSocket >
		{
			friend class g2::SocketBase< UDPSocket >;
			
		public:
			UDPSocket();
			~UDPSocket();

			void Connect( const char *addr, unsigned short port );
			void EnableBroadcast( bool flag );

			ssize_t SendTo( const char *buf, size_t size, const struct sockaddr &destAddr, socklen_t addrLen );
			ssize_t ReceiveFrom( char *buf, size_t size, struct sockaddr &srcAddr, socklen_t &addrLen );

		private:
			ssize_t SendImpl( const char *buf, size_t size );
			ssize_t SendImpl( const struct iovec *iov, size_t count );
			ssize_t ReceiveImpl( char *buf, size_t size );
			ssize_t ReceiveImpl( struct iovec *iov, size_t count );
			
			g2::AddrInfo addrInfo_;

			static const int SOCKET_TYPE = SOCK_DGRAM;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__UDPSocketTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/udpsocket.h\
$(here)/udpsocket.cpp
</Makefile.am>
*/

#include "eventtype.h"

using namespace g2;

int GutsEntryUDPSocketTest(int,char**)
	{
	char buf[1600];
	UDPSocket sock;

	sock.Bind( "0.0.0.0", 56000 );

	struct sockaddr_in skadin;
	memset( &skadin, 0, sizeof( skadin ) );
	socklen_t len = sizeof( skadin );
	NetUtil::SetSockAddrIn( &skadin, "192.168.11.1", 56000 );
	
	printf( "%d\n",
			sock.SendTo( "A", 1, (struct sockaddr&)skadin, len ) );
	
	
	return 0;
	}

#endif
#endif
