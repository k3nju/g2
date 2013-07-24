#include "udpsocket.h"
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	UDPSocket::UDPSocket()
		:SocketBase< UDPSocket >()
		,addrInfo_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	UDPSocket::~UDPSocket()
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
	void UDPSocket::SetDestination( const Destination &dst )
		{
		int ret = 0;
		if( addrInfo_.Set( dst.family, SOCKET_TYPE, AddrInfo::CLIENT, dst.host, dst.service, ret ) == false )
			{
			string msg = "getaddrinfo() failed. " + string( gai_strerror( ret ) );
			throw NetworkException( msg, ret, NAME_RESOLUTION_FAILURE );
			}
		}
	
	//-----------------------------------------------------------------------------------------//
	void UDPSocket::EnableBroadcast( bool flag )
		{
		int flagInt = (int)flag;
		SetOption( SOL_SOCKET, SO_BROADCAST, &flagInt, sizeof( flagInt ) );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::SendImpl( const char *buf, size_t size )
		{
		assert( addrInfo_.Get() != NULL && "call Connect() before call me" );
		const struct addrinfo *res = addrInfo_.Get();
		
		return sendto( GetSocket(), buf, size, MSG_NOSIGNAL, res->ai_addr, res->ai_addrlen );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::SendImpl( const struct iovec*, size_t )
		{
		assert( "Not implemented on UPSocket" );
		
		return -1;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::ReceiveImpl( char *buf, size_t size )
		{
		struct sockaddr_in skadIn;
		socklen_t len = sizeof( skadIn );
		memset( &skadIn, 0, sizeof( skadIn ) );

		ssize_t result = recvfrom( GetSocket(), buf, size, 0, (struct sockaddr*)(&skadIn), &len );
		
		const struct addrinfo *ai = addrInfo_.Get();
		assert( ai != NULL && "call Connect() before call me or use ReceiveFrom() insted if you intend as server socket" );
		if( addrInfo_.IsSame( (struct sockaddr&)skadIn, len ) == false )
			{
			throw NetworkException( "received from unconnected host", 0, RECEIVE_FAILURE );
			}

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::ReceiveImpl( struct iovec*, size_t )
		{
		assert( "Not implemented on UPSocket" );
		
		return -1;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::SendTo( const char *buf, size_t size, const struct sockaddr &destAddr, socklen_t addrLen )
		{
		ssize_t result = 0;

		do
			{
			result = sendto( GetSocket(), buf, size, 0, &destAddr, addrLen );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "SentTo() failed", errno, SEND_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );

		return result;
		}
	
	//-----------------------------------------------------------------------------------------//
	ssize_t UDPSocket::ReceiveFrom( char *buf, size_t size, struct sockaddr &srcAddr, socklen_t &addrLen )
		{
		ssize_t result = 0;

		do
			{
			result = recvfrom( GetSocket(), buf, size, 0, &srcAddr, &addrLen );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "ReceiveFrom() failed", errno, RECEIVE_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );

		return result;
		}
	}

