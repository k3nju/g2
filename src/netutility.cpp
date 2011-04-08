#include "netutility.h"
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <errno.h>
#include "criticalscope.h"
#include "networkexception.h"

namespace g2
	{
	g2::MutexLock NetUtility::mLock_;

	//-----------------------------------------------------------------------------------------//
	void NetUtility::SetSockAddrIn( struct sockaddr_in *skadIn, const char *ip, u_short port )
		{
		// TODO: Need Lock?
		CriticalScope< MutexLock > cScope( NetUtility::mLock_ );
		struct in_addr nbo_ip;

		if( inet_aton( ip, &nbo_ip ) != 0 )
			{
			skadIn->sin_addr = nbo_ip;
			}
		else
			{
			struct hostent *hostent = gethostbyname( ip );
			if( hostent == NULL )
				{
				throw NetworkException( "gethostbyname() failed", errno, NAME_RESOLUTION_FAILURE );
				}
			skadIn->sin_addr.s_addr = *(u_long*)(hostent->h_addr_list[0]);
			}

		skadIn->sin_family = AF_INET;
		skadIn->sin_port   = htons( port );
		}

	//-----------------------------------------------------------------------------------------//
	struct sockaddr_in* NetUtility::GetSockName( struct sockaddr_in *skadIn, int sock )
		{
		int size = sizeof( struct sockaddr_in );

		if( getsockname( sock, reinterpret_cast< struct sockaddr* >( skadIn ), reinterpret_cast< socklen_t* >( &size ) ) == -1 )
			{
			throw NetworkException( "getsockname() failed", errno, GET_SOCKNAME_FAILURE );
			}

		return skadIn;
		}

	//-----------------------------------------------------------------------------------------//
	struct sockaddr_in* NetUtility::GetPeerName( struct sockaddr_in *skadIn, int sock )
		{
		int size = sizeof( struct sockaddr_in );

		if( getpeername( sock, reinterpret_cast< struct sockaddr* >( skadIn ), reinterpret_cast< socklen_t* >( &size ) ) == -1 )
			{
			throw NetworkException( "getpeername() failed", errno, GET_PEERNAME_FAILURE );
			}

		return skadIn;
		}

	//-----------------------------------------------------------------------------------------//
	string NetUtility::GetIP( const struct sockaddr_in *skadIn )
		{
		// TODO: separate mLock.
		CriticalScope< MutexLock > cScope( NetUtility::mLock_ );
		return string( inet_ntoa( skadIn->sin_addr ) );
		}

	//-----------------------------------------------------------------------------------------//
	u_short NetUtility::GetPort( const struct sockaddr_in * skadIn )
		{
		return ntohs( skadIn->sin_port );
		}

	//-----------------------------------------------------------------------------------------//
	int NetUtility::CreateSocket( int domain, int type, int protocol )
		{
		int tmpsock = socket( domain, type, protocol );
		if( tmpsock == -1 )
			{
			throw NetworkException( "socket() failed", errno, SOCKET_FAILURE );
			}

		return tmpsock;
		}

	//-----------------------------------------------------------------------------------------//
	void NetUtility::SetSockOpt( int sock, int level, int optname, const void *value, socklen_t len )
		{
		if( setsockopt( sock, level, optname, value, len ) == -1 )
			{
			throw NetworkException( "setsockopt() failed", errno, SET_SOCKOPT_FAILURE );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void NetUtility::GetSockOpt( int sock, int level, int optname, void *value, socklen_t *len )
		{
		if( getsockopt( sock, level, optname, value, len ) == -1 )
			{
			throw NetworkException( "getsockpot() failed", errno, GET_SOCKOPT_FAILURE );
			}
		}

	//-----------------------------------------------------------------------------------------//
	int NetUtility::EnableNonBlocking( int sock, bool flag )
		{
		if( flag == true )
			{
			return fcntl( sock, F_SETFL, O_NONBLOCK );
			}
		else
			{
			return fcntl( sock, F_SETFL, fcntl( sock, F_GETFL ) & ~O_NONBLOCK );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void NetUtility::EnableReuseAddr( int sock, bool flag )
		{
		int f = (int)flag;
		SetSockOpt( sock, SOL_SOCKET, SO_REUSEADDR, &f, sizeof( f ) );
		}
	}












