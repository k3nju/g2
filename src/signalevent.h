#pragma once

#include "uncopyable.h"
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

			void Signal();
			void Broadcast();
			void Wait();
			int Wait( time_t sec, long int n_sec );

		private:
			g2::MutexLock lock_;
			g2::ConditionValue cond_;
		};
	}
