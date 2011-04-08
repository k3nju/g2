#include "conditionvalue.h"
#include <errno.h>
#include "mutexlock.h"
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	ConditionValue::ConditionValue( MutexLock &mlock )
		:mLock_( &mlock )
		{
		pthread_cond_init( &condValue_, NULL );
		}

	//-----------------------------------------------------------------------------------------//
	ConditionValue::~ConditionValue()
		{
		pthread_cond_destroy( &condValue_ );
		}

	//-----------------------------------------------------------------------------------------//
	void ConditionValue::Signal()
		{
		if( pthread_cond_signal( &condValue_ ) != 0 )
			{
			throw Exception( "ConditionValue::Signal() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void ConditionValue::Broadcast()
		{
		if( pthread_cond_broadcast( &condValue_ ) != 0 )
			{
			throw Exception( "ConditionValue::Broadcast() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void ConditionValue::Wait()
		{
		if( pthread_cond_wait( &condValue_, &( mLock_->lock_ ) ) != 0 )
			{
			throw Exception( "ConditionValule::Wait() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	int ConditionValue::Wait( time_t sec, long int n_sec )
		{
		timespec time = { sec, n_sec };
		int ret = 0;
		
		do
			{
			ret = pthread_cond_timedwait( &condValue_, &( mLock_->lock_ ), &time );
			}
		while( ret == EINTR );

		return ret;
		}
	}
