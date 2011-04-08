#pragma once

#include <sys/epoll.h>
#include "uncopyable.h"

namespace g2
	{
	template < size_t Size >
	class EpollEvent :private g2::Uncopyable< EpollEvent< Size > >
		{
		public:
			EpollEvent();
			~EpollEvent();
			
			inline size_t GetSize();
			inline epoll_event* GetRaw();

			inline void SetValidSize( size_t valid_size );
			inline size_t GetValidSize() const;

			inline int GetFDAt( size_t index ) const;
			inline int GetEventsAt( size_t index ) const;

		private:
			size_t validSize_;
			epoll_event events_[ Size ];
		};

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	EpollEvent< Size >::EpollEvent()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	EpollEvent< Size >::~EpollEvent()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	size_t EpollEvent< Size >::GetSize()
		{
		return Size;
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	epoll_event* EpollEvent< Size >::GetRaw()
		{
		return events_;
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	void EpollEvent< Size >::SetValidSize( size_t valid_size )
		{
		validSize_ = valid_size;
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	size_t EpollEvent< Size >::GetValidSize() const
		{
		return validSize_;
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	int EpollEvent< Size >::GetFDAt( size_t index ) const
		{
		return events_[ index ].data.fd;
		}

	//-----------------------------------------------------------------------------------------//
	template < size_t Size >
	int EpollEvent< Size >::GetEventsAt( size_t index ) const
		{
		return events_[ index ].events;
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__EpollEventTest

using namespace g2;

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devel/src/epolleventwr.h\
/home/kizi/devel/src/epolleventwr.cpp
</Makefile.am>
*/

int GutsEntryEpollEventTest( int, char** )
	{
	EpollEvent< 4 > E;
	E.GetSize();
	E.GetRaw();
	E.SetValidSize( 9 );
	E.GetValidSize();

	return 0;
	}

#endif
#endif

