#include "fdbase.h"
#include <errno.h>
#include <unistd.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	FDBase::FDBase()
		:fd_( -1 )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	FDBase::FDBase( int fd )
		:fd_( fd )
		{
		}

	//-----------------------------------------------------------------------------------------//
	FDBase::~FDBase()
		{
		if( fd_ != -1 )
			{
			Close();
			}
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t FDBase::Read( void *buf, size_t size )
		{
		ssize_t retval = 0;

		do
			{
			retval = read( fd_, buf, size );
			if( retval == -1 && errno != EINTR )
				{
				return -1;
				}
			}
		while( retval == -1 && errno == EINTR );
		
		return retval;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t FDBase::Write( void *buf, size_t size )
		{
		ssize_t retval = 0;

		do
			{
			retval = write( fd_, buf, size );
			if( retval == -1 && errno != EINTR )
				{
				return -1;
				}
			}
		while( retval == -1 && errno == EINTR );

		return retval;
		}

	//-----------------------------------------------------------------------------------------//
	int FDBase::Close()
		{
		return close( fd_ );
		}
	}
