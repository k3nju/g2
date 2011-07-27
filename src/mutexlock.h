#pragma once

#include <pthread.h>
#include "conditionvalue.h"
#include "uncopyable.h"

namespace g2
	{
	class MutexLock
		{
			G2_MARK_UNCOPYABLE( MutexLock );
			
		public:
			MutexLock();
			MutexLock( int type );
			~MutexLock();
			
			void Acquire();
			void Release();
			bool TryAcquire();

		private:
			pthread_mutex_t lock_;
			pthread_mutexattr_t attr_;
			
			friend class g2::ConditionValue;
		};
	}

