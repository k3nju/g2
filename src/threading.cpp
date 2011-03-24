#include "threading.h"
#include "exception.h"
#include <cassert>
#include <errno.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Threading::Threading()
		:resultCode_( -1 ),
		 isDetached_( false ),
		 isJoined_( true ),
		 threadId_( 0 ),
		 sigset_()
		{
		thArgs_.self = this;
		thArgs_.args = NULL;
		sigset_.Fill();
		}
	
	//-----------------------------------------------------------------------------------------//
	Threading::~Threading()
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	void Threading::Create( const pthread_attr_t *attr )
		{
		if( isJoined_ == false )
			{
			return;
			}
		
		isDetached_ = isJoined_ = false;
		isRunning_ = true;

		int result = pthread_create( &threadId_, attr,
									 static_cast< void* (*)(void*) >( &Threading::ThreadRoutine ),
									 static_cast< void* >( &thArgs_ ) );
		
		if( result != 0 )
			{
			isRunning_ = false;
			throw Exception( "pthread_create() failed", errno, result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Threading::Join()
		{
		if( isDetached_ == true || isJoined_ == true )
			{
			return;
			}
		
		int result = pthread_join( threadId_, NULL );
		if( result != 0 )
			{
			throw Exception( "pthread_join() failed", errno, result );
			}

		isJoined_ = true;
		}

	//-----------------------------------------------------------------------------------------//
	void Threading::Cancel()
		{
		int result = pthread_cancel( threadId_ );
		if( result != 0 )
			{
			throw Exception( "pthread_cancel() failed", errno, result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Threading::EnableCancel( bool flag )
		{
		int oldstate = 0;
		int newstate = flag == true ? PTHREAD_CANCEL_ENABLE : PTHREAD_CANCEL_DISABLE;
		
		pthread_setcancelstate( newstate, &oldstate );
		}

	//-----------------------------------------------------------------------------------------//
	int Threading::GetResultCode() const
		{
		return resultCode_;
		}

	//-----------------------------------------------------------------------------------------//
	void Threading::Detach()
		{
		int result = pthread_detach( threadId_ );
		if( result != 0 )
			{
			throw Exception( "pthread_detach() failed", errno, result );
			}

		isDetached_ = true;
		}

	//-----------------------------------------------------------------------------------------//
	void Threading::SetArgs( void *args )
		{
		thArgs_.args = args;
		}

	//-----------------------------------------------------------------------------------------//
	pthread_t Threading::GetThreadID() const
		{
		return threadId_;
		}

	//-----------------------------------------------------------------------------------------//
	pthread_t Threading::GetThreadID()
		{
		return threadId_;
		}
	
	//-----------------------------------------------------------------------------------------//
	bool Threading::IsRunning() const
		{
		return __sync_bool_compare_and_swap( &isRunning_, true, true );
		}

	//-----------------------------------------------------------------------------------------//
	g2::Sigset& Threading::GetSigset()
		{
		return sigset_;
		}

	//-----------------------------------------------------------------------------------------//
	void* Threading::ThreadRoutine( void *args )
		{
		assert( args != NULL && "args is NULL" );
		detail::ThreadingArgsType *thArgs = static_cast< detail::ThreadingArgsType* >( args );
		Threading *self = thArgs->self;
		assert( self != NULL && "self is NULL" );

		// TODO: is AddMask correct ? SetMask?
		self->GetSigset().AddMask();
		self->resultCode_ = self->Thread( thArgs->args );
		
		__sync_bool_compare_and_swap( &( self->isRunning_ ), true, false );
		
		pthread_exit( NULL );
		
		return NULL;
		}

	//-----------------------------------------------------------------------------------------//
	TaskThreading::TaskThreading( thread_func_t &thread_func )
		:ThreadFunc_( thread_func )
		{
		}

	//-----------------------------------------------------------------------------------------//
	TaskThreading::~TaskThreading()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void TaskThreading::SetThreadFunction( thread_func_t &thread_func )
		{
		ThreadFunc_ = thread_func;
		}
	
	//-----------------------------------------------------------------------------------------//
	int TaskThreading::Thread( void *args )
		{
		return ThreadFunc_( args );
		}	
	}
