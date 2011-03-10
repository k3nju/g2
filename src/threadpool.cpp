#include "threadpool.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	ThreadPool::ThreadPool()
		:threads_(),
		 threadsLock_,
		 stop_( false )
		{
		}

	//-----------------------------------------------------------------------------------------//
	ThreadPool::~ThreadPool()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	bool ThreadPool::Add( worker_prt_t worker, void *argv, pthread_t *tid )
		{
		if( IsStoppped() == true ||
			worker->IsRunning() == true )
			{
			return false;
			}
		
		worker->SetArgv( 
		pthread_t tmp = 0;
		
		
		pthread_t tid = worker->GetThreadID();
		critical_scope_t locked( threadsLock_ );
		threads_.insert( std::make_pair( newTid, worker ) );

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	void ThreadPool::Join()
		{
		
		}
	}
