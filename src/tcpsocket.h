#pragma once

#include "socketbase.h"

namespace g2
	{
	class TCPSocket :public g2::SocketBase< TCPSocket >
		{
			friend class g2::SocketBase< TCPSocket >;
			
		public:
			TCPSocket();
			explicit TCPSocket( int socket );
			~TCPSocket();

			void Connect( const char *addr, unsigned short port );
			bool EndConnect();
			
			void Listen( int backlog );
			int Accept();
			void ShutDown( int flag );

		private:
			ssize_t SendImpl( const char *buf, size_t size );
			ssize_t SendImpl( const struct iovec *iov, size_t count );
			ssize_t ReceiveImpl( char *buf, size_t size );
			ssize_t ReceiveImpl( struct iovec *iov, size_t count );

			static const int SOCKET_TYPE = SOCK_STREAM;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__TCPSocketTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/tcpsocket.h\
$(here)/tcpsocket.cpp
</Makefile.am>
*/

#include "iodispatch.h"
#include "socketheaders.h"
#include <cassert>
#include <deque>
using namespace std;
using namespace g2;


int GutsEntryTCPSocketTest( int, char** )
	{
	/*
	TCPSocket sock;
	sock.Connect( "www.microsoft.com", 80 );
	Send( sock );
	{
	TCPSocket rsock = Receive( sock );
	rsock.Close();
	assert( rsock.GetSocket() != -1 );
	}

	assert( sock.GetSocket() != -1 );
	sock.Close();
	assert( sock.GetSocket() == -1 );

	TCPSocket sock2;
	for( int i = 0; i < 10; ++ i )
		{
		sock.Connect( "www.microsoft.com", 80 );
		Send( sock );
		sock = Receive( sock );
		//assert( sock.GetSocket() == -1 );
		}
	*/

	puts( "[*] first" );
	TCPSocket sock;
	sock.EnableReuseAddr( true );
	sock.Bind( "localhost", 56000 );
	sock.Listen(1);
	sock.Accept();
	sock.Close();
	puts( "[*] second" );
	sock.CreateSocket( SOCK_STREAM );
	sock.EnableReuseAddr( true );
	sock.Bind( "localhost", 56000 );
	sock.Listen(1);
	sock.Close();
	

	return 0;
	}

#endif
#endif




