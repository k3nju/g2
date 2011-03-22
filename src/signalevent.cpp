#include "signalevent.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	SignalEvent::SignalEvent()
		:lock_(),
		 cond_( lock_ )
		{
		}

	//-----------------------------------------------------------------------------------------//
	SignalEvent::~SignalEvent()
		{
		}

	//-----------------------------------------------------------------------------------------//
	int SignalEvent::Signal()
		{
		CriticalScope<> locked( lock_ );
		return cond_.Signal();
		}

	//-----------------------------------------------------------------------------------------//
	int SignalEvent::Broadcast()
		{
		CriticalScope<> locked( lock_ );
		return cond_.Broadcast();
		}

	//-----------------------------------------------------------------------------------------//
	int SignalEvent::Wait()
		{
		CriticalScope<> locked( lock_ );
		return cond_.Wait();
		}

	//-----------------------------------------------------------------------------------------//
	int SignalEvent::Wait( time_t sec, long int n_sec )
		{
		CriticalScope<> locked( lock_ );
		return cond_.Wait( sec, n_sec );
		}
	}
