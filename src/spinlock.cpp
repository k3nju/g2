#include "spinlock.h"
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	SpinLock::SpinLock()
		{
		pthread_spin_init( &lock_, PTHREAD_PROCESS_PRIVATE );
		}

	//-----------------------------------------------------------------------------------------//
	SpinLock::~SpinLock()
		{
		pthread_spin_destroy( &lock_ );
		}

	//-----------------------------------------------------------------------------------------//
	void SpinLock::Acquire()
		{
		int result = pthread_spin_lock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "SpinLock::Acquire() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void SpinLock::Release()
		{
		int result = pthread_spin_unlock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "SpinLock::Release() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool SpinLock::TryAcquire()
		{
		int result = pthread_spin_trylock( &lock_ );
		if( result == 0 )
			{
			return true;
			}
		else if( result = EBUSY )
			{
			return false;
			}

		throw Exception( "SpinLock::TryAcquire() failed", result );
		}
	}
