#include "threadpool.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	WorkerArgs::WorkerArgs( ThreadPool *threadPool )
		:threadPool_( threadPool ),
		 run_( true )
		{
		}

	//-----------------------------------------------------------------------------------------//
	ThreadPool::ThreadPool()
		:threads_(),
		 threadsLock_(),
		 isStopped_( false ),
		 stopEvent_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	ThreadPool::~ThreadPool()
		{
		}

	//-----------------------------------------------------------------------------------------//
	pthread_t ThreadPool::AddImpl( threading_ptr_t thread )
		{
		worker_ptr_t worker( new worker_t( thread, this ) );
		thread->SetArgs( &(worker->wargs) );
		thread->Create();
		pthread_t tid = thread->GetThreadID();
		
		critical_scope_t locked( threadsLock_ );
		threads_.insert( std::make_pair( tid, worker ) );
		
		return tid;
		}

	//-----------------------------------------------------------------------------------------//
	void ThreadPool::Remove( pthread_t tid )
		{
		critical_scope_t locked( threadsLock_ );
		if( threads_.empty() || pthread_self() == tid )
			{
			return;
			}
		
		thread_map_t::iterator iter = threads_.find( tid );
		if( iter == threads_.end() )
			{
			return;
			}
		
		iter->second->wargs.Stop();
		iter->second->thread->Join();
		threads_.erase( iter );
		}

	//-----------------------------------------------------------------------------------------//
	void ThreadPool::Stop()
		{
		__sync_bool_compare_and_swap( &isStopped_, false, true );
		stopEvent_.Signal();
		}
	
	//-----------------------------------------------------------------------------------------//
	void ThreadPool::Join()
		{
		if( __sync_and_and_fetch( &isStopped_, true ) == false )
			{
			stopEvent_.Wait();
			}
			
		critical_scope_t locked( threadsLock_ );
		thread_map_t::iterator begin = threads_.begin();
		thread_map_t::iterator end = threads_.end();
		
		for(; begin != end; ++begin )
			{
			begin->second->wargs.Stop();
			}
		
		begin = threads_.begin();
		for(; begin != end; ++begin )
			{
			begin->second->thread->Join();
			}
		}
	}

