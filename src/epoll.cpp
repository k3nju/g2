#include "epoll.h"
#include <errno.h>
#include <unistd.h>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Epoll::Epoll( unsigned int size )
		:epollFd_( -1 )
		{
		epollFd_ = epoll_create( size );
		if( epollFd_ == -1 )
			{
			throw Exception( "epoll_create() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	Epoll::~Epoll()
		{
		close( epollFd_ );
		}

	//-----------------------------------------------------------------------------------------//
	void Epoll::Register( int fd, int events, const epoll_data_t *data )
		{
		if( Ctrl( EPOLL_CTL_ADD, fd, events, data ) == -1 )
			{
			throw Exception( "Epoll::Register() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Epoll::Update( int fd, int events, const epoll_data_t *data )
		{
		if( Ctrl( EPOLL_CTL_MOD, fd, events, data ) == -1 )
			{
			throw Exception( "Epoll::Update() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Epoll::Unregister( int fd )
		{
		int result = epoll_ctl( epollFd_, EPOLL_CTL_DEL, fd, NULL );
		if( result == -1 && errno != ENOENT )
			{
			throw Exception( "Epoll::Unregister() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	int Epoll::Select( epoll_event *events, size_t eventsSize, int timeout )
		{
		int result = epoll_wait( epollFd_, events, eventsSize, timeout );
		if( result == -1 && errno != EINTR )
			{
			throw Exception( "Epoll::Select() failed", errno );
			}

		return result;
		}
	
	//-----------------------------------------------------------------------------------------//
	int Epoll::Ctrl( int op, int fd, int events, const epoll_data_t *data )
		{
		struct epoll_event eve;
		eve.events = events;
		if( data != NULL )
			eve.data = *data;

		return epoll_ctl( epollFd_, op, fd, &eve );
		}
	}

