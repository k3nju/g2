#ifndef GENSIN__EventFDH
#define GENSIN__EventFDH

#include <sys/eventfd.h>
#include "uncopyable.h"

namespace g2
	{
	class EventFD :private g2::Uncopyable
		{
		public:
			EventFD( unsigned int initial_value );
			~EventFD();
			
			eventfd_t Read();
			void Write( eventfd_t value );
			void Close();

			int GetFD() const { return fd_; };
			void EnableNonBlocking( bool flag );

		private:
			int fd_;
		};
	}

#endif

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__EventFDTest

#include <stdio.h>
#include "epoll.h"
#include "eventtype.h"
#include "epolleventwr.h"

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/eventfd.h \
$(here)/eventfd.cpp
</Makefile.am>
 */

using namespace g2;

int GutsEntryEventFDTest(int,char**)
	{
	EventFD EveFD( 0 );
	Epoll E(10);

	E.Register( EveFD.GetFD(), event_type::READ );
	EveFD.Write( 0xfd );

	EpollEventWr<10> eevent;
	
	E.Select( &eevent, -1 );
	printf( "0x%x\n", EveFD.Read() );

	return 0;
	}

#endif
#endif

