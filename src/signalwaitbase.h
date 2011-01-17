#pragma once

#include <signal.h>
#include <pthread.h>
#include <errno.h>
#include "sigset.h"
#include "exception.h"

namespace g2
	{
	template < class Derived >
	class SignalWaitBase
		{
		public:
			void Mask();
			void Wait();

		protected:
			SignalWaitBase();
			SignalWaitBase( const SignalWaitBase &rhs );
			const SignalWaitBase& operator = ( const SignalWaitBase &rhs );
			~SignalWaitBase();

			g2::Sigset sigset_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	SignalWaitBase< Derived >::SignalWaitBase():sigset_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	SignalWaitBase< Derived >::SignalWaitBase( const SignalWaitBase &rhs )
		{
		sigset_ = rhs.sigset_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	const SignalWaitBase< Derived >& SignalWaitBase< Derived >::operator = ( const SignalWaitBase< Derived > &rhs )
		{
		if( this != &rhs )
			{
			sigset_ = rhs.sigset_;
			}
		
		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	SignalWaitBase< Derived >::~SignalWaitBase()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	void SignalWaitBase< Derived >::Mask()
		{
		if( pthread_sigmask( SIG_BLOCK, &( const_cast< sigset_t& >( sigset_.Get() ) ), NULL ) != 0 )
			{
			throw Exception( "pthread_sigmask() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class Derived >
	void SignalWaitBase< Derived >::Wait()
		{
		int signalNumber = 0;
		if( sigwait( &( const_cast< sigset_t& >( sigset_.Get() ) ), &signalNumber ) != 0 )
			{
			throw Exception( "sigwait() failed" );
			}

		static_cast< Derived* >( this )->Handle( signalNumber );
		}
	
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SignalWaitBaseTest

#include <stdio.h>

using namespace g2;

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devel/g2x/src/sigset.h\
/home/kizi/devel/g2x/src/sigset.cpp\
/home/kizi/devel/g2x/src/signalwaitbase.h\
/home/kizi/devel/g2x/src/signalwaitbase.cpp
</Makefile.am>
 */

class Derived :public SignalWaitBase< Derived >
	{
	public:
		void Initialize()
			{
			sigset_.Fill(); // handle all signals
			}
		
//  private:
		void Handle( int signalNumber )
			{
			printf( "signal caught = %d\n", signalNumber );
			}
	};

int GutsEntrySignalWaitBaseTest( int, char ** )
	{
	Derived d;
	d.Initialize();
	d.Mask();
	d.Wait();
	
	return 0;
	}

#endif
#endif
