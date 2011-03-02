#pragma once

#include "mutexlock.h"
#include "uncopyable.h"

namespace g2
	{
	class ThreadSafeFlag :private g2::Uncopyable< ThreadSafeFlag >
		{
		public:
			ThreadSafeFlag( bool flag );			
			ThreadSafeFlag();
			~ThreadSafeFlag();
					
			void Set( bool flag );
			bool Get();
			bool Get() const;
	
			bool operator = ( bool flag );
			operator bool ();

		private:
			bool flag_;

			mutable g2::MutexLock lock_;
		};
	
	typedef ThreadSafeFlag AtomicFlag;
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ThreadSafeFlagTest

#include <stdio.h>

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/threadsafeflag.h
$(here)/threadsafeflag.cpp
</Makefile.am>
*/

using namespace g2;

int GutsEntryThreadSafeFlagTest(int, char** )
	{
	ThreadSafeFlag f( true );

	printf( "%d\n", f.Get() );
	f.Set( false );
	printf( "%d\n", f.Get() );

	return 0;
	}
	

#endif
#endif

