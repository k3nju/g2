#include "tcpsocket.h"
#include "addrinfo.h"
#include "utility.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	TCPSocket::TCPSocket():SocketBase< TCPSocket >()
		{
		}

	//-----------------------------------------------------------------------------------------//
	TCPSocket::TCPSocket( int socket ):SocketBase< TCPSocket >( socket )
		{
		}

	//-----------------------------------------------------------------------------------------//
	TCPSocket::~TCPSocket()
		{
		try
			{
			Close();
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	void TCPSocket::Connect( const AddrInfo &addrInfo )
		{
		const struct addrinfo *res = addrInfo.Get();
		
		if( GetSocket() == -1 )
			{
			SetSocket( NetUtil::CreateSocket( res->ai_family, res->ai_socktype, res->ai_protocol ) );
			}
		
		int result = 0;
		do
			{
			result = connect( GetSocket(), res->ai_addr, res->ai_addrlen );
			if( result == -1 && errno != EINTR && errno != EINPROGRESS && errno != EISCONN )
				{
				throw NetworkException( "connect() failed", errno, CONNECT_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );
		}

	//-----------------------------------------------------------------------------------------//
	bool TCPSocket::EndConnect()
		{
		char value = 0;
		socklen_t size = sizeof( value );
			
		GetOption( SOL_SOCKET, SO_ERROR, &value, &size );
		if( value != 0 && value != EISCONN )
			{
			return false;
			}

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	void TCPSocket::Listen( int backlog )
		{
		if( listen( GetSocket(), backlog ) == -1 )
			{
			throw NetworkException( "listen() failed", errno, LISTEN_FAILURE );
			}
		}

	//-----------------------------------------------------------------------------------------//
	int TCPSocket::Accept()
		{
		int client_socket = 0;
			
		client_socket = accept( GetSocket(), NULL, NULL );
		if( client_socket == -1 && errno != EAGAIN )
			{
			throw NetworkException( "accept() failed", errno, ACCEPT_FAILURE );
			}

		return client_socket;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t TCPSocket::SendImpl( const char *buf, size_t size )
		{
		return send( GetSocket(), buf, size, MSG_NOSIGNAL );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t TCPSocket::SendImpl( const struct iovec *iov, size_t size )
		{
		return writev( GetSocket(), iov, size );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t TCPSocket::ReceiveImpl( char *buf, size_t size )
		{
		return recv( GetSocket(), buf, size, 0 );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t TCPSocket::ReceiveImpl( struct iovec *iov, size_t count )
		{
		return readv( GetSocket(), iov, count );
		}

	//-----------------------------------------------------------------------------------------//
	void TCPSocket::ShutDown( int flag )
		{
		if( shutdown( GetSocket(), flag ) == -1 )
			{
			throw NetworkException( "shutdown() failed", errno, SHUTDOWN_FAILURE );
			}
		}
	}
