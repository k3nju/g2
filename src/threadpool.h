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
	class IThreadPool
		{
		public:
			virtual pthread_t Add( void *args ) = 0;
			virtual void Remove( pthread_t tid ) = 0;
			virtual void Join() = 0;
			virtual void Stop() = 0;
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

			void Stop(){ __sync_bool_compare_and_swap( &run_, true, false ); }
			bool IsRunnable() { __sync_fetch_and_and( &run_, true ); }
			
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
			G2_MARK_UNCOPYABLE( ThreadPool< Worker > );
			
			class PoolableThreading :public Worker
				{
					using Worker::SetArgs;
					
				public:
					PoolableThreading( IThreadPool *threadPool, void *args )
						:args_( threadPool, args )
						{
						SetArgs( &args_ );
						}
					~PoolableThreading()
						{
						puts( "[*] dtor" );
						}

					void Stop(){ args_.Stop(); }
					
				private:
					WorkerArgs args_;
				};
			
			typedef boost::shared_ptr< PoolableThreading > worker_ptr_t;
			typedef std::map< pthread_t, worker_ptr_t > thread_map_t;
			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;
			
		public:
			ThreadPool();
			~ThreadPool();
			
			virtual pthread_t Add( void *args );
			virtual void Remove( pthread_t tid );
			virtual void Join();
			virtual void Stop(){ stopEvent_.Signal(); }
			
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
	pthread_t ThreadPool< Worker >::Add( void *args )
		{
		worker_ptr_t worker( new PoolableThreading( this, args ) );
		worker->Create();
		pthread_t tid = worker->GetThreadID();
		
		critical_scope_t locked( threadsLock_ );
		threads_.insert( std::make_pair( tid, worker ) );

		return tid;
		}

	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	void ThreadPool< Worker >::Remove( pthread_t tid )
		{
		critical_scope_t locked( threadsLock_ );
		if( threads_.empty() )
			return;

		typename thread_map_t::iterator iter = threads_.find( tid );
		if( iter->second->GetThreadID() == pthread_self() )
			{
			throw Exception( "ThreadPool< Worker >::Remove() can't stop self thread" );
			}
		
		iter->second->Stop();
		iter->second->Join();
		threads_.erase( iter );
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class Worker >
	void ThreadPool< Worker >::Join()
		{
		stopEvent_.Wait();

		critical_scope_t locked( threadsLock_ );
		typename thread_map_t::iterator begin = threads_.begin();
		typename thread_map_t::iterator end = threads_.end();

		for(; begin != end; ++begin )
			{
			begin->second->Stop();
			}

		begin = threads_.begin();
		for(; begin != end; ++begin )
			{
			begin->second->Join();
			}
		}
	}
