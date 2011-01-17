#include "threadsafeflag.h"
#include "criticalscope.h"
#include <stdio.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	ThreadSafeFlag::ThreadSafeFlag( bool flag )
		:flag_( flag ),
		 lock_()
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	ThreadSafeFlag::ThreadSafeFlag()
		:flag_( false ),
		 lock_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	ThreadSafeFlag::~ThreadSafeFlag()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void ThreadSafeFlag::Set( bool flag )
		{
		CriticalScope< MutexLock > Locked( lock_ );
		
		flag_ = flag;
		}

	//-----------------------------------------------------------------------------------------//
	bool ThreadSafeFlag::Get() const
		{
		return static_cast< const ThreadSafeFlag* >( this )->Get();
		}
	
	//-----------------------------------------------------------------------------------------//
	bool ThreadSafeFlag::Get()
		{
		CriticalScope< MutexLock > Locked( lock_ );
		
		return flag_;
		}

	//-----------------------------------------------------------------------------------------//
	bool ThreadSafeFlag::operator = ( bool flag )
		{
		CriticalScope< MutexLock > Locked( lock_ );
		
		return flag_ = flag;
		}

	//-----------------------------------------------------------------------------------------//
	ThreadSafeFlag::operator bool ()
		{
		CriticalScope< MutexLock > Locked( lock_ );
		
		return flag_;
		}
	}
