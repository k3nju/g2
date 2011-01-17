#include "sigset.h"
#include <errno.h>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Sigset::Sigset()
		{
		Empty();
		}

	//-----------------------------------------------------------------------------------------//
	Sigset::Sigset( const Sigset &rhs )
		{
		sigset_ = rhs.sigset_;
		}

	//-----------------------------------------------------------------------------------------//
	const Sigset& Sigset::operator = ( const Sigset &rhs )
		{
		if( this != &rhs )
			{
			sigset_ = rhs.sigset_;
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	Sigset::~Sigset()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::Empty()
		{
		if( sigemptyset( &sigset_ ) == -1 )
			{
			throw Exception( "sigemptyset() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::Fill()
		{
		if( sigfillset( &sigset_ ) == -1 )
			{
			throw Exception( "sigfillset() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::Add( int signalNumber )
		{
		if( sigaddset( &sigset_, signalNumber ) == -1 )
			{
			throw Exception( "sigaddset() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::Remove( int signalNumber )
		{
		if( sigdelset( &sigset_, signalNumber ) == -1 )
			{
			throw Exception( "sigdelset() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool Sigset::IsMember( int signalNumber )
		{
		int result = sigismember( &sigset_, signalNumber );
		if( result == -1 )
			{
			throw Exception( "sigismember() failed", errno );
			}

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	const sigset_t& Sigset::Get() const
		{
		return sigset_;
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::SetMask()
		{
		Mask( SIG_SETMASK );
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::AddMask()
		{
		Mask( SIG_BLOCK );
		}

	//-----------------------------------------------------------------------------------------//
	void Sigset::Mask( int how )
		{
		int result = pthread_sigmask( how, &sigset_, NULL );
		if( result != 0 )
			{
			throw Exception( "pthread_sigmask() failed", result );
			}
		}
	}



