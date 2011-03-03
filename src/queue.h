#pragma once

#include <queue>
#include "mutexlock.h"
#include "criticalscope.h"
#include "conditionvalue.h"
#include "uncopyable.h"

namespace g2
	{
	using namespace std;

	template < class T > class Queue :private g2::Uncopyable< Queue< T > >
		{
		public:
			typedef T data_t;
			typedef std::queue< T > std_queue_t;
			
			Queue();

			void Enqueue( const T& item );
			
			// if queue has no object to pop,
			// queue will block until new object is pushed
			T Dequeue();

			bool IsEmpty() const;
			unsigned int GetSize();
			void Clear();

			// for scoped operation
			inline void StartScopedOperation();
			inline void EndScopedOperation();
			inline std_queue_t& GetStdQueue();
			
			void Signal();
			void Signal( size_t count );
			void Broadcast();
			void Wait();

		private:
			g2::MutexLock qLock_;
			g2::ConditionValue condValue_;
			std_queue_t queue_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T > Queue< T >::Queue()
		:qLock_(),
		 condValue_( qLock_ ),
		 queue_()
		{}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Enqueue( const T &item )
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		queue_.push( item );
		condValue_.Signal();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > T Queue< T >::Dequeue()
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		while( queue_.empty() == true )
			{
			condValue_.Wait();
			}

		T tmp = queue_.front();
		queue_.pop();

		return tmp;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > bool Queue< T >::IsEmpty() const
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		return queue_.empty();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > unsigned int Queue< T >::GetSize()
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		return queue_.size();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Clear()
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		size_t size = queue_.size();
		for( size_t i = 0; i < size; ++i )
			{
			queue_.pop();
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::StartScopedOperation()
		{
		qLock_.Acquire();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::EndScopedOperation()
		{
		qLock_.Release();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > typename Queue< T >::std_queue_t& Queue< T >::GetStdQueue()
		{
		return queue_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Signal()
		{
		condValue_.Signal();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Signal( size_t count )
		{
		while( count > 0 )
			{
			condValue_.Signal();
			--count;
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Broadcast()
		{
		condValue_.Broadcast();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > void Queue< T >::Wait()
		{
		CriticalScope< MutexLock > cScope( qLock_ );
		
		condValue_.Wait();
		}
		
	}

