#include "endpoint.h"
#include "string.h"
#include "netutility.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	void EndPoint::Init()
		{
		memset( &address, 0, sizeof( address ) );
		}

	//-----------------------------------------------------------------------------------------//
	void EndPoint::Init( const char *ip, unsigned short port )
		{
		memset( &address, 0, sizeof( address ) );
		g2::NetUtil::SetSockAddrIn( &address, ip, port );
		}

	//-----------------------------------------------------------------------------------------//
	void EndPoint::Init( int sock )
		{
		memset( &address, 0, sizeof( address ) );
		g2::NetUtil::GetPeerName( &address, sock );
		}

	//-----------------------------------------------------------------------------------------//
	const std::string EndPoint::GetIP() const
		{
		return g2::NetUtil::GetIP( &address );
		}

	//-----------------------------------------------------------------------------------------//
	unsigned short EndPoint::GetPort() const
		{
		return g2::NetUtil::GetPort( &address );
		}

	//-----------------------------------------------------------------------------------------//
	bool operator == ( const EndPoint &lhs, const EndPoint &rhs )
		{
		return (bool)( memcmp( &lhs, &rhs, sizeof( lhs ) ) == 0 );
		}
	}
