#include "randomint.h"
#include <string.h>
#include <errno.h>
#include "exception.h"
#include "criticalscope.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	RandomInt::RandomInt()
		:fp_( NULL ), cacheIndex_( 0 )
		{
		memset( cache_, 0, sizeof( cache_ ) / sizeof( cache_[0] ) );
		
		fp_ = fopen( "/dev/urandom", "rb" );
		/*
		if( fp__ == NULL )
			{
			throw Exception( "fopen( /dev/urandom ) failed", errno );
			}
		*/
		
		FillCache();
		}

	//-----------------------------------------------------------------------------------------//
	RandomInt::~RandomInt()
		{
		if( fp_ != NULL )
			{
			fclose( fp_ );
			}
		}

	//-----------------------------------------------------------------------------------------//
	int RandomInt::Get()
		{
		g2::CriticalScope< g2::MutexLock > locked( lock_ );
		
		if( cacheIndex_ >= ( sizeof( cache_ ) / sizeof( cache_[0] ) ) )
			{
			FillCache();
			cacheIndex_ = 0;
			}
		
		return cache_[ cacheIndex_++ ];
		}
	
	//-----------------------------------------------------------------------------------------//
	void RandomInt::FillCache()
		{
		if( fp_ != NULL )
			{
			if( fread( cache_,
					   sizeof( cache_[0] ),
					   sizeof( cache_ ) / sizeof( cache_[0] ),
					   fp_ ) != sizeof( cache_ ) / sizeof( cache_[0] ) )
				{
				memset( cache_, 0, sizeof( cache_ ) / sizeof( cache_[0] ) );
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	int GetRandomInt()
		{
		static RandomInt randInt;
		
		return randInt.Get();
		}
	}
