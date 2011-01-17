#pragma once

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/function.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/bind.hpp>
#include "exception.h"
#include "threadcontrolstate.h"
#include "scopedoperation.h"

namespace g2
	{
	using namespace std;

	template < class WorkerType >
	class ThreadPool :private g2::Uncopyable
		{

		public:
			typedef WorkerType worker_t;
			typedef typename worker_t::data_t data_t;
			typedef boost::shared_ptr< worker_t > worker_ptr_t;
			typedef g2::ThreadControlState< data_t > thread_control_state_t;
			
			explicit ThreadPool( size_t thread_count );
			~ThreadPool();

			void Begin();
			void AddThread();
			void End();
			void EndImmediately();
			void RemoveThread();
			void WaitAll();
			void MarkInactive( const worker_ptr_t &worker );

			void Enqueue( data_t &data );
			data_t Dequeue();
			size_t GetQueuedSize();
			
			size_t GetActiveWorkerCount() const;
			size_t GetWorkerCount() const;
			
			bool IsFinished();

			thread_control_state_t& GetThreadControlState();
			
		private:
			typedef list< worker_ptr_t > worker_list_t;
			typedef queue< worker_ptr_t > inactive_queue_t;

			bool isRunning_;
			size_t activesCount_;
			
			worker_list_t workers_;
			inactive_queue_t inactiveWorkers_;
			mutable g2::MutexLock lock_;
			thread_control_state_t controlChannel_;
		};


	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	ThreadPool< WorkerType >::ThreadPool( size_t threadCount )
		:isRunning_( false )
		,activesCount_( 0 )
		,workers_()
		,inactiveWorkers_()
		,lock_()
		,controlChannel_()
		{
		assert( threadCount > 0 );
		
		for( size_t i = 0; i < threadCount; ++i )
			{
			worker_ptr_t Worker( new worker_t() );
			Worker->SetArgv( this );
			workers_.push_back( Worker );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	ThreadPool< WorkerType >::~ThreadPool()
p		{
		try
			{
			End();
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::Begin()
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );
		
		if( isRunning_ == true )
			{
			throw Exception( "now ThreadPool is running" );
			}

		isRunning_ = true;
		typename worker_list_t::iterator head = workers_.begin();
		typename worker_list_t::iterator foot = workers_.end();
		
		for(; head != foot; ++head )
			{
			(*head)->Create();
			controlChannel_.Add( (*head)->GetThreadID() );
			++activesCount_;
			}
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::AddThread()
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );
		
		if( isRunning_ == false )
			{
			return;
			}

		worker_ptr_t worker;
		if( inactiveWorkers_.empty() == false )
			{
			worker = inactiveWorkers_.front();
			inactiveWorkers_.pop();
			
			worker->Join();
			}
		else
			{
			worker.reset( new worker_t() );
			workers_.push_back( worker );
			}
		
		worker->SetArgv( this );
		worker->Create();
		controlChannel_.Add( Worker->GetThreadID() );
		++activesCount_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >	
	void ThreadPool< WorkerType >::RemoveThread()
		{
		CriticalScope< g2::MutexLock > CSocpe( lock_ );
		
		if( isRunning_ == false )
			{
			return;
			}

		if( workers_.size() <= 1 )
			{
			return;
			}

		typename worker_list_t::iterator head = workers_.begin();
		controlChannel_.SetControlState( (*head)->GetThreadID(), thread_control_states::END );
		--runningSize_;
		}	

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >	
	void ThreadPool< WorkerType >::End()
		{
		CriticalScope< g2::MutexLock > CSocpe( lock_ );
		
		if( isRunning_ == true )
			{
			return;
			}

		isRunning_ = true;

		controlChannel_.SetControlState( thread_control_states::END_WHEN_NO_DATA );
		controlChannel_.WaitUntilAllDequeued();
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::EndImmediately()
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );

		if( isRunning_ == true )
			{
			return;
			}

		isRunning_ = true;
		
		controlChannel_.Clear();
		controlChannel_.SetControlState( thread_control_states::END );
		controlChannel_.WaitUntilAllDequeued();
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::WaitAll()
		{
		typename worker_list_t::iterator head = workers_.begin();
		typename worker_list_t::iterator foot = workers_.end();

		for(; head != foot; ++head )
			{
			(*head)->Join();
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::MarkInactive( const worker_ptr_t &worker )
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );

		controlChannel_.Remove( worker->GetThreadID() );
		inactiveWorkers_.push( worker );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	void ThreadPool< WorkerType >::Enqueue( data_t &data )
		{
		controlChannel_.Enqueue( data );
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	typename ThreadPool< WorkerType >::data_t ThreadPool< WorkerType >::Dequeue()
		{
		return controlChannel_.Dequeue();
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	size_t ThreadPool< WorkerType >::GetQueuedSize()
		{
		return controlChannel_.GetQueuedSize();
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	size_t ThreadPool< WorkerType >::GetRunningSize() const
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );
		
		return runningSize_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	size_t ThreadPool< WorkerType >::GetWorkerSize() const
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );
		
		return workers_.size();
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	bool ThreadPool< WorkerType >::IsEnd()
		{
		CriticalScope< g2::MutexLock > cScope( lock_ );

		return isRunning_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class WorkerType >
	typename ThreadPool< WorkerType >::thread_control_state_t& ThreadPool< WorkerType >::GetThreadControlState()
		{
		return controlChannel_;
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ThreadPoolTest

#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <stdlib.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <functional>
#include "threading.h"
#include "exception.h"
using namespace g2;

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/threadpool.h\
$(here)/threadpool.cpp
</Makefile.am>
 */

class Test;

typedef ThreadPool< Test > threadpool_t;

class Test :public g2::Threading
	{
	public:
		typedef int data_t;
		
		virtual int Thread( void *argv )
			{
			int i = 0;
			threadpool_t *tp = (threadpool_t*)argv;

			while( tp->GetThreadControlState().GetControlState( GetThreadID(), i ) != thread_control_states::END )
				{
				printf( "%x:%d\n", GetThreadID(), i );
				
				sleep( rand() % 10 );
				}

			return 0;
			}
	};

int GutsEntryThreadPoolTest( int, char** )
	{

	threadpool_t tp(10);

	tp.Begin();
	for( int i = 0; i < 100; ++i )
		{
		tp.Enqueue( i );
		}

	tp.End();
	tp.WaitAll();
	
	
	return 0;
	}

#endif
#endif

