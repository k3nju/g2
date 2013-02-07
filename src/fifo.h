#pragma once

#include <sys/types.h>
#include <string>
#include "uncopyable.h"
#include "fdbase.h"

namespace g2
	{
	using namespace std;
	
	class Fifo :public FDBase
		{
			G2_MARK_UNCOPYABLE( Fifo );

		public:
			Fifo();
			Fifo( const char *pathname, mode_t fifoMode, int openFlags );
			~Fifo();
			
			void Open( const char *pathname, mode_t fifoMode, int openFlags );
		};
	}
