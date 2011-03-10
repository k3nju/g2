#pragma once

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
	struct ThreadArgs
		{
			void *argv;
			ThreadPool *threadPool;
		};
	
	//-----------------------------------------------------------------------------------------//
	class ThreadPool
		{
			G2_MARK_UNCOPYABLE( self_t );

			typedef boost::shared_ptr< g2::Threading > worker_ptr_t;
			typedef std::map< pthread_t, worker_ptr_t > thread_map_t;
			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;
			
		public:
			ThreadPool();
			~ThreadPool();

			bool Add( worker_ptr_t worker, void *argv, pthread_t *tid );
			void Join();
			
			inline void Stop(){ __sync_fetch_and_or( &stop_, true ); }
			inline bool IsStopped() const { return __sync_fetch_and_and( &stop_, true ); }
			
		private:
			
			
			thread_map_t threads_;
			g2::MutexLock threadsLock_;
			
			bool stop_;
		};
	}
