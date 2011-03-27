#include "threadpool.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	WorkerArgs::WorkerArgs( IThreadPool *threadPool, void *args )
		:threadPool_( threadPool ),
		 args_( args ),
		 run_( true )
		{
		}
	}

