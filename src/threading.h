#pragma once

#include <pthread.h>
#include <boost/function.hpp>
#include "mutexlock.h"
#include "sigset.h"
#include "uncopyable.h"

namespace g2
	{
	class Threading;

	//-----------------------------------------------------------------------------------------//
	namespace detail
		{
		typedef struct ThreadingArgument
			{
				Threading *self;
				void *args;
			} ThreadingArgsType;
		}

	//-----------------------------------------------------------------------------------------//
	class Threading :private g2::Uncopyable< Threading >
		{
		public:
			Threading();
			virtual ~Threading();

			void Create( const pthread_attr_t *attr = NULL );
			void Join();
			void Cancel();
			int GetResultCode() const;
			void Detach();

			void SetArgs( void *args );
			
			pthread_t GetThreadID() const;
			pthread_t GetThreadID();
			bool IsRunning() const;

			g2::Sigset& GetSigset();
			
		protected:
			virtual int Thread( void *args ) = 0;
			void EnableCancel( bool flag );
			
		private:
			static void* ThreadRoutine( void *args );

			int resultCode_;
			bool isDetached_;
			bool isJoined_;
			mutable bool isRunning_;
			pthread_t threadId_;
			g2::Sigset sigset_;
			detail::ThreadingArgsType thArgs_;
		};

	//-----------------------------------------------------------------------------------------//
	class TaskThreading :public Threading
		{
		public:
			typedef boost::function<int (void*)> thread_func_t;
			
			TaskThreading( thread_func_t &thread_func );
			virtual ~TaskThreading();
			
			void SetThreadFunction( thread_func_t &thread_func );

		private:
			virtual int Thread( void *args );
			
			thread_func_t ThreadFunc_;			
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ThreadingTest
/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devel/gensinx/g2/src/threading.h		\
/home/kizi/devel/gensinx/g2/src/threading.cpp
</Makefile.am>
 */

#include <stdio.h>
#include <unistd.h>
#include <boost/function.hpp>
#include "criticalscope.h"
#include "conditionvalue.h"
#include "mutexlock.h"

using namespace g2;

g2::MutexLock Lock;
g2::ConditionValue cv(Lock);

struct C 
	{
	public:
		C(){puts("C");};
		~C(){puts("~C()");};
	};

int Func( void *p )
	{
	printf( "%x\n", (size_t)p );
	sleep( 1 );
	puts( "calling wait" );
	cv.Wait();
	puts( "wait returned" );

	return 0;
	}

int F( void *p )
	{
	C c;
	sleep(10);
	
	return 0;
	}

int GutsEntryThreadingTest( int, char** )
	{
	TaskThreading::thread_func_t d( &F );
	TaskThreading ts( d );
	ts.Create();
	ts.Join();
	
	/*
	char buf[100];
	TaskThreading::thread_func_t d( &Func );
	
	TaskThreading TestThread( d );
	TestThread.GetSigset().Fill();
	TestThread.SetArgs( reinterpret_cast< void* >( 0xff ) );
	TestThread.Create();
	cv.Signal();
	puts( "signaled" );
	

	TestThread.Join();

	printf( "result = %x\n", TestThread.GetResultCode() );

	*/
	return 0;
	}

#endif
#endif
