#pragma once

#include <errno.h>
#include <sys/epoll.h>
#include "exception.h"
#include "uncopyable.h"

namespace g2
	{
	using namespace std;

	class Epoll :private g2::Uncopyable< Epoll >
		{
		public:
			explicit Epoll( unsigned int size = 1024 );
			~Epoll();

			void Register( int fd, int events, const epoll_data_t *data );
			void Update( int fd, int events, const epoll_data_t *data );
			void Unregister( int fd );
			
			int Select( epoll_event *events, size_t eventsSize, int timeout );
			
			int GetFD() const { return epollFd_; };
			
		private:
			int Ctrl( int op, int fd, int events, const epoll_data_t *data );

			int epollFd_;
		};
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__EpollTest

#include <stdio.h>
#include <vector>
#include "tcpsocket.h"
#include "netutility.h"
#include "exception.h"
#include "epollevent.h"
#include "eventtype.h"

/*
<Makefile.am>
EPOLL_DIR=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(EPOLL_DIR)/epoll.h \
$(EPOLL_DIR)/epoll.cpp
</Makefile.am>
 */

using namespace std;
using namespace g2;
using namespace g2;


int GutsEntryEpollTest( int, char** )
	{
	// TODO: write usage
	
	return 0;
	}


#endif
#endif
