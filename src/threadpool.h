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
	class ThreadArgs
		{
		public:
			ThreadArgs( uint32_t &status, void *argv )
				:status_( status )
				{
				}
			
			inline void SetStatus( uint32_t newStatus ){ while( __sync_bool_compare_and_swap( &status_, &status_, newStatus ) == false ); }
			inline bool GetStatus(){ return __sync_fetch_and_or( &status_, status_ );  }
			inline void* GetArgv() const { return argv_; }
			
		private:
			uint32_t &status_;
			void *argv_;
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
			
		private:
			thread_map_t threads_;
			g2::MutexLock threadsLock_;
			
			int stop_;
		};
	}
