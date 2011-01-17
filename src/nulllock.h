#pragma once

#include "uncopyable.h"

namespace g2
	{
	class NULLLock
		{
		public:
			inline void Acquire(){};
			inline void Release(){};
			inline bool TryAcquire(){ return true; };
		};
	}
