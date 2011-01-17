#pragma once

#include <time.h>
#include <stdint.h>
#include <sys/timerfd.h>
#include "uncopyable.h"

namespace g2
	{
	class TimerFD :private g2::Uncopyable
		{
		public:
			TimerFD();
			~TimerFD();

			void Start( time_t initSec, long initNsec, time_t intrvSec, long intrvNsec );
			void StartOneShot( time_t sec, long nsec = 0 );
			void StartInterval( time_t sec, long nsec = 0 );
			void Stop();
			
			uint64_t Read();
			void Close();

			int GetFD() const;

		private:
			void SetTime( struct itimerspec &spec );
			
			int fd_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__TimerFDTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/timerfd.h\
$(here)/timerfd.cpp
guts_LDFLAGS=-lrt
</Makefile.am>
*/


#include <stdio.h>
using namespace g2;

int GutsEntryTimerFDTest(int,char**)
	{
	TimerFD tfd;
	tfd.StartInterval( 1 );

	for( size_t i = 0; i < 10; ++i )
		{
		printf( "%d\n", tfd.Read() );
		}

	tfd.Close();
	
	return 0;
	}

#endif
#endif
