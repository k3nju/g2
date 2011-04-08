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
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	class ThreadPool;

	//-----------------------------------------------------------------------------------------//
	class WorkerArgs
		{
		public:
			WorkerArgs( ThreadPool *threadPool );
			inline void Stop(){ __sync_bool_compare_and_swap( &run_, true, false ); }
			inline bool IsRunnable(){ return __sync_fetch_and_and( &run_, true ); }
			inline ThreadPool* GetThreadPool() const { return threadPool_; }
			
		private:
			ThreadPool *threadPool_;
			bool run_;
		};
	
	//-----------------------------------------------------------------------------------------//
	class ThreadPool
		{
			G2_MARK_UNCOPYABLE( ThreadPool );

			typedef boost::shared_ptr< g2::Threading > threading_ptr_t;
			typedef struct Worker
				{
					Worker( threading_ptr_t t, ThreadPool *tp ):thread( t ), wargs( tp ){}
					threading_ptr_t thread;
					WorkerArgs wargs;
				} worker_t;
			typedef boost::shared_ptr< worker_t > worker_ptr_t;
			typedef std::map< pthread_t, worker_ptr_t > thread_map_t;
			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;
			
		public:
			ThreadPool();
			~ThreadPool();

			template < class WorkerImpl >
			pthread_t Add();
			template < class WorkerImpl, class T0 >
			pthread_t Add( T0 &t0 );
			template < class WorkerImpl, class T0, class T1 >
			pthread_t Add( T0 &t0, T1 &t1 );
			template < class WorkerImpl, class T0, class T1, class T2 >
			pthread_t Add( T0 &t0, T1 &t1, T2 &t2 );
			template < class WorkerImpl, class T0, class T1, class T2, class T3 >
			pthread_t Add( T0 &t0, T1 &t1, T2 &t2, T3 &t3 );
			template < class WorkerImpl, class T0, class T1, class T2, class T3, class T4 >
			pthread_t Add( T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4 );
			
			void Remove( pthread_t tid );
			void Join();
			void Stop();
			
		private:
			pthread_t AddImpl( threading_ptr_t worker );
			
			thread_map_t threads_;
			g2::MutexLock threadsLock_;
			bool isStopped_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl >
	pthread_t ThreadPool::Add()
		{
		return AddImpl( threading_ptr_t( new WorkerImpl() ) );
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl, class T0 >
	pthread_t ThreadPool::Add( T0 &t0 )
		{
		return AddImpl( new WorkerImpl( t0 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl, class T0, class T1 >
	pthread_t ThreadPool::Add( T0 &t0, T1 &t1 )
		{
		return AddImpl( new WorkerImpl( t0, t1 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl, class T0, class T1, class T2 >
	pthread_t ThreadPool::Add( T0 &t0, T1 &t1, T2 &t2 )
		{
		return AddImpl( new WorkerImpl( t0, t1, t2) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl, class T0, class T1, class T2, class T3 >
	pthread_t ThreadPool::Add( T0 &t0, T1 &t1, T2 &t2, T3 &t3 )
		{
		return AddImpl( new WorkerImpl( t0, t1, t2, t3 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerImpl, class T0, class T1, class T2, class T3, class T4 >
	pthread_t ThreadPool::Add( T0 &t0, T1 &t1, T2 &t2, T3 &t3, T4 &t4 )
		{
		return AddImpl( new WorkerImpl( t0, t1, t2, t3, t4 ) );
		}
	}
