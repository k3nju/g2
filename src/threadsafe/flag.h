#pragma once

#include "../mutexlock.h"
#include "../uncopyable.h"

namespace g2
	{
	namespace threadsafe
		{
		class Flag :private g2::Uncopyable< Flag >
			{
			public:
				Flag( bool flag );
				Flag();
				~Flag();
					
				void Set( bool flag );
				bool Get();
				bool Get() const;
	
				bool operator = ( bool flag );
				operator bool ();

			private:
				bool flag_;

				mutable g2::MutexLock lock_;
			};
	
		typedef Flag AtomicFlag;
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__FlagTest

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

int GutsEntryFlagTest(int, char** )
	{
	Flag f( true );

	printf( "%d\n", f.Get() );
	f.Set( false );
	printf( "%d\n", f.Get() );

	return 0;
	}
	

#endif
#endif

