#include "eventfd.h"
#include <errno.h>
#include "netutility.h"
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	EventFD::EventFD( unsigned int initial_value ):fd_( -1 )
		{
		fd_ = eventfd( initial_value, 0 );
		if( fd_ == -1 )
			{
			throw Exception( "eventfd() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	EventFD::~EventFD()
		{
		try
			{
			Close();
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	eventfd_t EventFD::Read()
		{
		eventfd_t value = 0;
		if( eventfd_read( fd_, &value ) == -1 )
			{
			throw Exception( "eventfd_read() failed", errno );
			}

		return value;
		}

	//-----------------------------------------------------------------------------------------//
	void EventFD::Write( eventfd_t value )
		{
		if( eventfd_write( fd_, value ) == -1 )
			{
			throw Exception( "eventfd_write() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void EventFD::Close()
		{
		if( close( fd_ ) == -1 )
			{
			throw Exception( "close() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void EventFD::EnableNonBlocking( bool flag )
		{
		NetUtil::EnableNonBlocking( fd_, flag );
		}
	}
