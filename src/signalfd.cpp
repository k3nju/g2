#include "signalfd.h"

#include <errno.h>
#include <cassert>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	SignalFD::SignalFD():fd_( -1 )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	SignalFD::SignalFD( const g2::Sigset &sigset ):fd_( -1 )
		{
		fd_ = signalfd( -1, &( sigset.Get() ), 0 );
		if( fd_ == -1 )
			{
			throw Exception( "signalfd() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	SignalFD::SignalFD( int fd, const g2::Sigset &sigset ):fd_( -1 )
		{
		fd_ = signalfd( fd, &( sigset.Get() ), 0 );
		if( fd_ == -1 )
			{
			throw Exception( "signalfd() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	SignalFD::~SignalFD()
		{
		if( fd_ != -1 )
			{
			try
				{
				Close();
				}
			catch( ... )
				{
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	void SignalFD::Set( const g2::Sigset &sigset )
		{
		if( fd_ != -1 )
			{
			Close();
			}

		fd_ = signalfd( -1, &( sigset.Get() ), 0 );
		if( fd_ == -1 )
			{
			throw Exception( "signalfd() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	const signalfd_siginfo& SignalFD::Read()
		{
		assert( fd_ != -1 && "fd_ is not initialized" );

		int result = 0;
		do
			{
			int result = read( fd_, &sigInfo_, sizeof( sigInfo_ ) );
			if( result == -1 && errno != EINTR )
				{
				throw Exception( "read() failed", errno );
				}
			}
		while( result == -1 && errno == EINTR );

		return sigInfo_;
		}

	//-----------------------------------------------------------------------------------------//
	void SignalFD::Close()
		{
		if( close( fd_ ) == -1 )
			{
			throw Exception( "close() failed", errno );
			}

		fd_ = -1;
		}	
	}
