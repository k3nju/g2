#pragma once

#include "uncopyable.h"
#include "criticalscope.h"
#include "mutexlock.h"
#include "conditionvalue.h"

namespace g2
	{
	class SignalEvent
		{
			G2_MARK_UNCOPYABLE( SignalEvent );
			
		public:
			SignalEvent();
			~SignalEvent();

			int Signal();
			int Broadcast();
			int Wait();
			int Wait( time_t sec, long int n_sec );

		private:
			g2::MutexLock lock_;
			g2::ConditionValue cond_;
		};
	}
