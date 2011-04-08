#pragma once

#include <boost/functional/hash.hpp>
#include <tr1/unordered_map>
#include <cassert>
#include <netinet/in.h>

namespace g2
	{
	struct EndPoint
		{
		public:
			struct sockaddr_in address;
			
			void Init();
			void Init( const char *ip, unsigned short port );
			void Init( int sock );
			const std::string GetIP() const;
			unsigned short GetPort() const;

			static const socklen_t ADDRESS_SIZE = sizeof( struct sockaddr_in );
		}
		__attribute__((packed));

	// for std::tr1::unordered_map
	bool operator == ( const EndPoint &lhs, const EndPoint &rhs );
	}

// ADL for std::tr1::unordered_map
namespace std
	{
	namespace tr1
		{
		template <>
		struct hash< g2::EndPoint >
			{
				size_t operator()( const g2::EndPoint &ep ) const
					{
					assert( sizeof( ep.address.sin_addr.s_addr ) == sizeof( size_t ) );
					size_t seed = 0;
					boost::hash_combine( seed, ep.address.sin_addr.s_addr );
					boost::hash_combine( seed, ep.address.sin_port );
					
					return seed;
					}
			};
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__EndPointTest

#include <tr1/type_traits>
#include <tr1/unordered_map>

using namespace std;
using namespace g2;

int GutsEntryEndPointTest(int,char**)
	{
	assert( tr1::is_pod< EndPoint >::value == 1 );
	assert( sizeof( EndPoint ) == EndPoint::ADDRESS_SIZE );

	return 0;
	}

#endif
#endif
