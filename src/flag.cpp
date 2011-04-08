#include "flag.h"
#include "criticalscope.h"

namespace g2
	{
	namespace threadsafe
		{
		//-----------------------------------------------------------------------------------------//
		Flag::Flag( bool flag )
			:flag_( flag ),
			 lock_()
			{
			}
	
		//-----------------------------------------------------------------------------------------//
		Flag::Flag()
			:flag_( false ),
			 lock_()
			{
			}

		//-----------------------------------------------------------------------------------------//
		Flag::~Flag()
			{
			}

		//-----------------------------------------------------------------------------------------//
		void Flag::Set( bool flag )
			{
			CriticalScope< MutexLock > Locked( lock_ );
		
			flag_ = flag;
			}

		//-----------------------------------------------------------------------------------------//
		bool Flag::Get() const
			{
			return static_cast< const Flag* >( this )->Get();
			}
	
		//-----------------------------------------------------------------------------------------//
		bool Flag::Get()
			{
			CriticalScope< MutexLock > Locked( lock_ );
		
			return flag_;
			}

		//-----------------------------------------------------------------------------------------//
		bool Flag::operator = ( bool flag )
			{
			CriticalScope< MutexLock > Locked( lock_ );
		
			return flag_ = flag;
			}

		//-----------------------------------------------------------------------------------------//
		Flag::operator bool ()
			{
			CriticalScope< MutexLock > Locked( lock_ );
		
			return flag_;
			}
		}
	}
