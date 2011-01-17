#pragma once
#include <sys/epoll.h>

namespace g2
	{
	struct EpollData
		{
			inline EpollData( void *ptr ):data(){ data.ptr = ptr; };
			inline EpollData( int fd ):data(){ data.fd = fd; };
			inline EpollData( __uint32_t u32 ):data(){ data.u32 = u32; };
			inline EpollData( __uint64_t u64 ):data(){ data.u64 = u64; };
			
			operator epoll_data_t (){ return data; };
			operator epoll_data_t* (){ return &data; };

			epoll_data_t data;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__EpollDataTest

#include <assert.h>

void Ptr( epoll_data_t *e )
	{
	assert( e->ptr == (void*)1 );
	}

void Fd( epoll_data_t *e )
	{
	assert( e->fd == 1 );
	}

void U32( epoll_data_t e )
	{
	assert( e.u32 == 0xffffffff );
	}

void U64( epoll_data_t e )
	{
	assert( e.u64 == 0xffffffffffffffffLL );
	}

int GutsEntryEpollDataTest( int, char** )
	{
	using namespace g2;
	
	EpollData p1( (void*)1 );
	Ptr( p1 );

	EpollData p2( 1 );
	Fd( p2 );

	EpollData p3( (__uint32_t)0xffffffff );
	U32( p3 );

	EpollData p4( (__uint64_t)0xffffffffffffffff );
	U64( p4 );
	
	return 0;
	}

#endif
#endif

