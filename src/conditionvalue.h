#pragma once

#include <pthread.h>
#include "uncopyable.h"

namespace g2
	{
	class MutexLock;

	//-----------------------------------------------------------------------------------------//
	class ConditionValue
		{
			G2_MARK_UNCOPYABLE( ConditionValue );
			
		public:
			explicit ConditionValue( g2::MutexLock &mlock );
			~ConditionValue();

			void Signal();
			void Broadcast();
			void Wait();
			int Wait( time_t sec, long int n_sec );
			
		private:
			g2::MutexLock *mLock_;
			pthread_cond_t condValue_;
		};
	}
