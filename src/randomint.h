#pragma once

#include <sys/types.h>
#include <stdio.h>
#include "mutexlock.h"

namespace g2
	{
	class RandomInt
		{
		public:
			RandomInt();
			~RandomInt();

			int Get();

		private:
			void FillCache();

			FILE *fp_;
			size_t cacheIndex_;
			int cache_[512];

			g2::MutexLock lock_;
		};

	int GetRandomInt();
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__RandomIntTest

#include <cassert>

int GutsEntryRandomIntTest(int, char**)
	{
	g2::RandomInt ri;
	for( int i = 0; i < 0xffff; ++i )
		{
		printf( "%x\n", ri.Get() );
		}
	
	return 0;
	}

#endif
#endif
