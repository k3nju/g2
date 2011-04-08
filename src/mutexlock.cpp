#include "mutexlock.h"
#include <errno.h>
#include "exception.h"


namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	MutexLock::MutexLock()
		{
		pthread_mutexattr_init( &attr_ );
		pthread_mutexattr_settype( &attr_, PTHREAD_MUTEX_FAST_NP );
		pthread_mutex_init( &lock_, &attr_ );
		}

	//-----------------------------------------------------------------------------------------//
	MutexLock::~MutexLock()
		{
		pthread_mutex_destroy( &lock_ );
		pthread_mutexattr_destroy( &attr_ );
		}

	//-----------------------------------------------------------------------------------------//
	void MutexLock::Acquire()
		{
		int result = pthread_mutex_lock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "MutexLock::Acquire() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void MutexLock::Release()
		{
		int result = pthread_mutex_unlock( &lock_ );
		if( result != 0 )
			{
			throw Exception ( "MutexLock::Release() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool MutexLock::TryAcquire()
		{
		int result = pthread_mutex_trylock( &lock_ );
		if( result == 0 )
			{
			return true;
			}
		else if( result == EBUSY )
			{
			return false;
			}

		throw Exception( "MutexLock::TryAcquire() failed", result );
		}
	}
