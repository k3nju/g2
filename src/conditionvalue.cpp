#include "conditionvalue.h"
#include "mutexlock.h"

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
	int ConditionValue::Signal()
		{
		return pthread_cond_signal( &condValue_ );
		}

	//-----------------------------------------------------------------------------------------//
	int ConditionValue::Broadcast()
		{
		return pthread_cond_broadcast( &condValue_ );
		}

	//-----------------------------------------------------------------------------------------//
	int ConditionValue::Wait()
		{
		return pthread_cond_wait( &condValue_, &( mLock_->lock_ ) );
		}

	//-----------------------------------------------------------------------------------------//
	int ConditionValue::Wait( time_t sec, long int n_sec )
		{
		timespec time = { sec, n_sec };
		
		return pthread_cond_timedwait( &condValue_, &( mLock_->lock_ ), &time );
		}
	}
