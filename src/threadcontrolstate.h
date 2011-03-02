#pragma once

#include <queue>
#include <tr1/unordered_map>
#include "criticalscope.h"

namespace g2
	{
	namespace thread_control_states
		{
		enum STATE
			{
			CONTINUE,
			END,
			END_WHEN_NO_DATA
			};
		}
	
	template < class T >
	class ThreadControlState :private g2::Uncopyable< ThreadControlState< T > >
		{
		public:
			typedef std::queue< T > queue_t;
			
			ThreadControlState();
			~ThreadControlState();
			
			void Add( pthread_t tid );
			void Remove( pthread_t tid );

			void StartScopedOperation();
			void EndScopedOperation();
			
			void SetControlState( pthread_t thread_id, thread_control_states::STATE state );
			void SetControlState( thread_control_states::STATE state );
			thread_control_states::STATE GetControlState( pthread_t thread_id );
			thread_control_states::STATE GetControlState( pthread_t thread_id, T &val_out );
			void Wait();
			void Wait( time_t sec, long int nSec );
			
			inline void Enqueue( T &val );
			inline void EnqueueWithoutLock( T &val );
			inline T Dequeue();
			inline T DequeueWithoutLock();
			inline size_t GetQueuedSize();
			inline size_t GetQueuedSizeWithoutLock();
			inline void Clear();
			queue_t& GetStdQueue();

		private:
			g2::MutexLock lock_;
			g2::ConditionValue condValue_;
			typedef std::tr1::unordered_map<pthread_t, thread_control_states::STATE> state_map_t;
			state_map_t states_;
			queue_t queue_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ThreadControlState< T >::ThreadControlState()
		:g2::Uncopyable(),
		 lock_(),
		 condValue_( lock_ ),
		 states_(),
		 queue_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ThreadControlState< T >::~ThreadControlState()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Add( pthread_t tid )
		{
		CriticalScope< MutexLock > locked( lock_ );

		states_[ tid ] = thread_control_states::CONTINUE;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Remove( pthread_t tid )
		{
		CriticalScope< MutexLock > locked( lock_ );

		state_map_t::iterator iter = states_.find( tid );
		if( iter != states_.end() )
			{
			states_.erase( iter );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Enqueue( T &val )
		{
		CriticalScope< MutexLock > locked( lock_ );
		
		EnqueueWithoutLock( val );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::EnqueueWithoutLock( T &val )
		{
		queue_.push( val );
		condValue_.Signal();
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class T >
	T ThreadControlState< T >::Dequeue()
		{
		CriticalScope< MutexLock > locked( lock_ );

		return DequeueWithoutLock();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T ThreadControlState< T >::DequeueWithoutLock()
		{
		T tmp = queue_.front();
		queue_.pop();

		return tmp;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	size_t ThreadControlState< T >::GetQueuedSize()
		{
		CriticalScope< MutexLock > locked( lock_ );

		return GetQueuedSizeWithoutLock();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	size_t ThreadControlState< T >::GetQueuedSizeWithoutLock()
		{
		return queue_.size();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Clear()
		{
		CriticalScope< MutexLock > locked( lock_ );
		
		while( queue_.empty() == false )
			{
			queue_.pop();
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >	
	void ThreadControlState< T >::SetControlState( pthread_t tid, thread_control_states::STATE state )
		{
		CriticalScope< MutexLock > locked( lock_ );

		states_[ tid ] = state;
		condValue_.Signal();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >	
	void ThreadControlState< T >::SetControlState( thread_control_states::STATE state )
		{
		CriticalScope< MutexLock > locked( lock_ );

		typename state_map_t::iterator head = states_.begin();
		typename state_map_t::iterator foot = states_.end();
		
		for(; head != foot; ++head )
			{
			head->second = state;
			}

		condValue_.Broadcast();
		}	

	//-----------------------------------------------------------------------------------------//
	template < class T >	
	void ThreadControlState< T >::StartScopedOperation()
		{
		lock_.Acquire();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::EndScopedOperation()
		{
		lock_.Release();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	thread_control_states::STATE ThreadControlState< T >::GetControlState( pthread_t tid )
		{
		typename state_map_t::iterator state = states_.find( tid );
		
		return state != states_.end() ? state->second : thread_control_states::CONTINUE;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	thread_control_states::STATE ThreadControlState< T >::GetControlState( pthread_t tid, T &valOut )
		{
		CriticalScope< MutexLock > locked( lock_ );
		
		while( true )
			{
			typename state_map_t::iterator state = states_.find( tid );
			if( state == states_.end() )
				{
				condValue_.Wait();
				continue;
				}
			
			if( state->second == thread_control_states::END )
				{
				return thread_control_states::END;
				}

			if( queue_.empty() == false )
				{
				valOut = queue_.front();
				queue_.pop();

				return thread_control_states::CONTINUE;
				}

			if( state->second == thread_control_states::END_WHEN_NO_DATA )
				{
				return thread_control_states::END;
				}

			condValue_.Wait();
			}

		return thread_control_states::CONTINUE;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Wait()
		{
		condValue_.Wait();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ThreadControlState< T >::Wait( time_t sec, long int nSec )
		{
		condValue_.Wait( sec, nSec );
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ThreadControlStateTest

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/threadcontrolstate.h \
$(here)/threadcontrolstate.cpp
</Makefile.am>
*/

#include "threading.h"
#include "scopedoperation.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace boost;
using namespace g2;
using namespace g2;

typedef ThreadControlState< int > TCS;

class Test :public Threading
	{
	public:
		Test( TCS *tcs ):tcs_( tcs )
			{
			}
		
		virtual int Thread( void *argv )
			{
			int i = 0;
			tcs_->Add( GetThreadID() );
/*
			printf( "ThreadStart = %x\n", GetThreadID() );
			{
			ScopedOperation< TCS > ScopedOp( tcs_ );
			
			while(true)
				{
				printf( "GetControlState = %x\n", GetThreadID() );
				if( tcs_->GetControlState( GetThreadID() ) == TCS::END )
					{
					printf( "returning = %x\n", GetThreadID() );					
					return 0;
					}

				printf( "calling GetQueuedSize = %x\n", GetThreadID() );				
				if( tcs_->GetQueuedSizeWithoutLock() > 0 )					
					{
					printf( "poping = %x\n", GetThreadID() );					
					i = tcs_->DequeueWithoutLock();
					printf( "%x = %d\n", GetThreadID(), i );
					}

				printf( "%x calling wait\n", GetThreadID() );
				tcs_->Wait();
				}
			}
*/

			while( tcs_->GetControlState( GetThreadID(), &i ) != thread_control_states::END )
				{
				printf( "i = %d on %x\n", i, GetThreadID() );
				}
			
			return 0;
			}

		TCS *tcs_;
	};

int GutsEntryThreadControlStateTest(int, char**)
	{
	TCS tcs;
	const int ts = 10;

	Test *array[ts];
	printf( "%d\n", sizeof(array)/4 );
	for( int i = 0; i < sizeof(array)/4; ++i )
		{
		array[i] = new Test( &tcs );
		array[i]->Create();
		
		}

	for( int i = ts; i >= 0; --i )
		{
		printf( "i = %d\n", i );
		tcs.Enqueue( i );
		}


	sleep(3);
	puts( "Set END" );	
	tcs.SetControlState( thread_control_states::END );

	for( int i = 0; i < sizeof(array)/4; ++i )
		{
		printf( "joining %x\n", array[i]->GetThreadID() );
		array[i]->Join();
		}

	for( int i = 0; i < ts; ++i )
		{
		delete array[i];
		}
/**/
	return 0;
	}

#endif
#endif
