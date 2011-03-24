#pragma once

#include <stdint.h>
#include <pthread.h>
#include <map>
#include <boost/shared_ptr.hpp>

#include "uncopyable.h"
#include "threading.h"
#include "mutexlock.h"
#include "criticalscope.h"
#include "signalevent.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	class IThreadPool
		{
		public:
			virtual bool Add( void *args, pthread_t *pthreadId ) = 0;
		};

	//-----------------------------------------------------------------------------------------//
	class WorkerArgs
		{
		public:
			WorkerArgs( IThreadPool *threadPool, void *args )
				:threadPool_( threadPool ),
				 args_( args ),
				 run_( true )
				{
				}
			
			void Stop(){ __sync_bool_compare_and_swap( &run_, false, true ); }
			bool IsRunnable() const { __sync_fetch_and_and( &run_, true ); }
			
			void* GetArgs() const { return args_; }
			IThreadPool* GetThreadPool() const { return threadPool_; }
			
		private:
			IThreadPool *threadPool_;
			void *args_;
			bool run_;
		};
	
	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	class ThreadPool :public IThreadPool
		{
			G2_MARK_UNCOPYABLE( self_t );
			
			class PoolableThreading :public Worker
				{
				public:
					PoolableThreading( IThreadPool *threadPool, void *args )
						:args_( threadPool, args )
						{
						SetArgs( &args_ );
						}
					
				private:
					WorkerArgs args_;
				};
			
			typedef boost::shared_ptr< PoolableThreading > worker_ptr_t;
			typedef std::map< pthread_t, worker_ptr_t > thread_map_t;
			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;
			
		public:
			ThreadPool();
			~ThreadPool();
			
			virtual bool Add( void *args, pthread_t *pthreadId );
			void Join();
			void Stop();
			
		private:
			thread_map_t threads_;
			g2::MutexLock threadsLock_;
			g2::SignalEvent stopEvent_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	ThreadPool< Worker >::ThreadPool()
		:threads_(),
		 threadsLock_(),
		 stopEvent_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	ThreadPool< Worker >::~ThreadPool()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	bool ThreadPool< Worker >::Add( void *args, pthread_t *pthreadId )
		{
		worker_ptr_t worker( new PoolableThreading( this, args ) );
		pthread_t newId = worker->Create();
		if( pthreadId != NULL )
			*pthreadId = newId;
		
		critical_scope_t locked( threadsLock_ );
		threads_.insert( std::make_pair( newId, worker ) );
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	void ThreadPool< Worker >::Join()
		{
		stopEvent_.Wait();
		
		thread_map_t::iterator begin = threads_.begin();
		thread_map_t::iterator end = threads_.end();

		for(;
		}
	}
