#pragma once

#include <stdint.h>
#include <map>
#include <pthread.h>
#include <boost/shared_ptr.hpp>
#include "uncopyable.h"
#include "threading.h"
#include "mutexlock.h"
#include "criticalscope.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	class ThreadPool;

	//-----------------------------------------------------------------------------------------//
	class WorkerArgs
		{
		public:
			WorkerArgs( void *argv )
				:argv_( argv ),
				 run_( true )
				{
				}
			
			void Stop(){ __sync_bool_compare_and_swap( &run_, false, true ); }
			bool IsRunnable() const { __sync_fetch_and_and( &run_, true ); }
			
			void* GetArgv() const { return argv_; }

		private:
			void *argv_;
			bool run_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	class ThreadPool
		{
			G2_MARK_UNCOPYABLE( self_t );

			typedef std::pair< Worker*, WorkerArgs* > entity_t;
			typedef std::map< pthread_t, entity_t > thread_map_t;
			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;
			
		public:
			typedef boost::shared_ptr< g2::Threading > worker_ptr_t;
			
			ThreadPool();
			~ThreadPool();
			
			bool Add( void *argv, pthread_t *pthreadId );
			void Join();
			
		private:
			thread_map_t threads_;
			g2::MutexLock threadsLock_;
			
			g2::ConditionValue stopEvent_;
			g2::MutexLock condLock_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	ThreadPool< Worker >::ThreadPool()
		:threads_(),
		 threadsLock_(),
		 run_( true )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	ThreadPool< Worker >::~ThreadPool()
		{
		thread_map_t::iterator begin = threads_.begin();
		thread_map_t::iterator end = threads_.end();

		for(; begin != end; ++begin )
			{
			delete begin.second.second;
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	bool ThreadPool< Worker >::Add( void *argv, pthread_t *pthreadId )
		{
		if( !__sync_fetch_and_and( &run_, true ) )
			{
			return false;
			}
		
		WorkerArgs *workerArgs = new WorkerArgs( run_, argv );
		worker_ptr_t worker( new Worker() );
		entity_t entity( worker, workerArgs );

		critical_scope_t locked( threadsLock_ );
		threads_.insert( entity );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	void ThreadPool< Worker >::Join()
		{
		
		}
	}
