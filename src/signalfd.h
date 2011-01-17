#pragma once

#include <sys/signalfd.h>
#include "sigset.h"
#include "uncopyable.h"

namespace g2
	{
	class SignalFD :private g2::Uncopyable
		{
		public:
			SignalFD();
			SignalFD( const g2::Sigset &sigset );
			SignalFD( int fd, const g2::Sigset &sigset );
			~SignalFD();

			void Set( const g2::Sigset &sigset );

			// don't support async read.
			// it may confuse you
			const signalfd_siginfo& Read();
			void Close();

			int GetFD() const { return fd_; };
			
		private:
			int fd_;
			signalfd_siginfo sigInfo_;
		};
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SignalFDTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/signalfd.h\
$(here)/signalfd.cpp
</Makefile.am>
*/

#include <stdio.h>
#include <signal.h>
#include <sys/signalfd.h>
#include "epollevent.h"
using namespace g2;

int GutsEntrySignalFDTest(int,char**)
	{
	Sigset Sigset;
	Sigset.Fill();

	sigprocmask( SIG_BLOCK, &( Sigset.Get() ), NULL );
	
	SignalFD sigfd( Sigset );

	const signalfd_siginfo &ss = sigfd.Read();
	printf( "\nsignal=%d\n", ss.ssi_signo );
	
	return 0;
	}


#endif
#endif

