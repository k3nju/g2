#pragma once

#include <pthread.h>
#include "uncopyable.h"

namespace g2
	{
	class SpinLock
		{
			G2_MARK_UNCOPYABLE( SpinLock );

		public:
			SpinLock();
			~SpinLock();

			void Acquire();
			void Release();
			bool TryAcquire();
			
		private:
			pthread_spinlock_t lock_;
		};
	}
