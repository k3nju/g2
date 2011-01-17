#pragma once

#include <pthread.h>
#include "uncopyable.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	class ReadWriteLock
		{
			G2_MARK_UNCOPYABLE( ReadWriteLock );
			
		public:
			ReadWriteLock();
			~ReadWriteLock();

			void AcquireReadLock();
			bool TryAcquireReadLock();
			
			void AcquireWriteLock();
			bool TryAcquireWriteLock();
			
			void Release();

		private:
			pthread_rwlock_t lock_;
		};

	//-----------------------------------------------------------------------------------------//
	class ReadLock
		{
			G2_MARK_UNCOPYABLE( ReadLock );
			
		public:
			ReadLock( ReadWriteLock &rwlock );
			
			void Acquire();
			void Release();
			bool TryAcquire();

		private:
			ReadWriteLock &rwlock_;
		};

	//-----------------------------------------------------------------------------------------//
	class WriteLock
		{
			G2_MARK_UNCOPYABLE( WriteLock );

		public:
			WriteLock( ReadWriteLock &rwlock );
			
			void Acquire();
			void Release();
			bool TryAcquire();

		private:
			ReadWriteLock &rwlock_;
		};
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ReadWriteLockTest

#include <cassert>
using namespace g2;

int GutsEntryReadWriteLockTest(int,char**)
	{
	ReadWriteLock rwlock;
	rwlock.AcquireReadLock();
	assert( rwlock.TryAcquireReadLock() == true );
	rwlock.Release();
	rwlock.Release();
	assert( rwlock.TryAcquireReadLock() == true );
	rwlock.Release();
	
	rwlock.AcquireWriteLock();
	assert( rwlock.TryAcquireWriteLock() == false );
	rwlock.Release();
	assert( rwlock.TryAcquireWriteLock() == true );
	rwlock.Release();

	{
	ReadLock rl( rwlock );
	rl.Acquire();
	assert( rl.TryAcquire() == true );
	rl.Release();
	rl.Release();
	assert( rl.TryAcquire() == true );
	rl.Release();
	}

	{
	
	WriteLock rl( rwlock );
	rl.Acquire();
	assert( rl.TryAcquire() == false );
	rl.Release();
	assert( rl.TryAcquire() == true );
	rl.Release();
	}
	
	return 0;
	}

#endif
#endif

