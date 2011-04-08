#include "signalevent.h"
#include "criticalscope.h"

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
	void SignalEvent::Signal()
		{
		CriticalScope<> locked( lock_ );
		return cond_.Signal();
		}

	//-----------------------------------------------------------------------------------------//
	void SignalEvent::Broadcast()
		{
		CriticalScope<> locked( lock_ );
		return cond_.Broadcast();
		}

	//-----------------------------------------------------------------------------------------//
	void SignalEvent::Wait()
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
