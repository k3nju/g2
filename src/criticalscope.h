#pragma once

#include "uncopyable.h"
#include "mutexlock.h"
#include <cassert>

namespace g2
	{
	template < class LockPolicy = g2::MutexLock >
	class CriticalScope :private g2::Uncopyable
		{
		public:
			explicit CriticalScope( LockPolicy &lock );
			CriticalScope(); // for trylock 
			~CriticalScope();

			void SetLock( LockPolicy &lock );
			void SetAcquiredLock( LockPolicy &lock );
			void Cancel();

		private:
			LockPolicy *lock_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	CriticalScope< LockPolicy >::CriticalScope( LockPolicy &lock )
		:lock_( &lock )
		{
		lock_->Acquire();
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	CriticalScope< LockPolicy >::CriticalScope()
		:lock_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	void CriticalScope< LockPolicy >::SetLock( LockPolicy &lock )
		{
		assert( lock_ == NULL );
		
		lock_ = &lock;
		lock_->Acquire();
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	void CriticalScope< LockPolicy >::SetAcquiredLock( LockPolicy &lock )
		{
		assert( lock_ == NULL );
		
		lock_ = &lock;
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	void CriticalScope< LockPolicy >::Cancel()
		{
		lock_->Release();
		lock_ = NULL;
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockPolicy >
	CriticalScope< LockPolicy >::~CriticalScope()
		{
		if( lock_ != NULL )
			{
			lock_->Release();
			}
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__CriticalScopeTest

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/criticalscope.h \
$(here)/criticalscope.cpp
$(here)/mutexlock.h \
$(here)/mutexlock.cpp
</Makefile.am>
*/

#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "mutexlock.h"

using namespace g2;

MutexLock lock;

double GetTime()
	{
	struct timeval tv;
	gettimeofday( &tv, NULL );

	return tv.tv_sec + (double)tv.tv_usec*1e-6;
	}

void A( int i )
	{
	int d = 0;
	for( int n = 0; n < i; ++n )
		{
		++d;
		}
	}

void B( int i )
	{
	CriticalScope< MutexLock> cScope( lock );
	int d = 0;
	for( int n = 0; n < i; ++n )
		{
		++d;
		}
	}

void PerformTest()
	{
		double start, end;
	start = end = 0;

	//-----------------------------------------------------------------------------------------//
	start = GetTime();
	for( size_t i = 0; i < 0xffff; ++i )
		{
		A( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	start = GetTime();
	for( size_t i = 0; i < 0xffff; ++i )
		{
		B( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	//-----------------------------------------------------------------------------------------//
	start = GetTime();
	for( size_t i = 0; i < 0xfffff; ++i )
		{
		A( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	start = GetTime();
	for( size_t i = 0; i < 0xfffff; ++i )
		{
		B( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	//-----------------------------------------------------------------------------------------//
	start = GetTime();
	for( size_t i = 0; i < 0xffffff; ++i )
		{
		A( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	start = GetTime();
	for( size_t i = 0; i < 0xffffff; ++i )
		{
		B( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	//-----------------------------------------------------------------------------------------//
	start = GetTime();
	for( size_t i = 0; i < 0xffffffff; ++i )
		{
		A( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );

	start = GetTime();
	for( size_t i = 0; i < 0xffffffff; ++i )
		{
		B( i );
		}
	end = GetTime();

	printf( "time = %10.30f\n", end - start );	
	}


int GutsEntryCriticalScopeTest( int, char ** )
	{
	g2::MutexLock mlock;
	mlock.Acquire();
	mlock.Acquire();
	puts( "Acquire" );
	
	return 0;
	}


#endif
#endif
