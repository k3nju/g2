#include "inotify.h"
#include "exception.h"
#include "utility.h"
#include "netutility.h"
#include <sys/inotify.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	INotify::INotify()
		:fd_( -1 ),
		 watchMap_()
		{
		fd_ = inotify_init();
		if( fd_ == -1 )
			{
			throw Exception( "inotify_init() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	INotify::~INotify()
		{
		try
			{
			Utility::Close( fd_ );
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	void INotify::Add( const char *path, uint32_t mask )
		{
		int wf = inotify_add_watch( fd_, path, mask );
		if( wf == -1 )
			{
			throw Exception( "inotify_add_watch() failed", errno );
			}

		watchMap_.insert( std::make_pair( path, wf ) );
		}

	//-----------------------------------------------------------------------------------------//
	void INotify::Remove( const char *path )
		{
		watch_map_t::iterator iter = watchMap_.find( path );
		if( iter == watchMap_.end() )
			{
			return;
			}

		if( inotify_rm_watch( fd_, iter->second ) == -1 )
			{
			throw Exception( "inotify_rm_watch() failed", errno );
			}

		watchMap_.erase( iter );
		}

	//-----------------------------------------------------------------------------------------//
	void INotify::Close()
		{
		Utility::Close( fd_ );
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t INotify::Read( char *buf, size_t size )
		{
		return Utility::Read( fd_, buf, size );
		}

	//-----------------------------------------------------------------------------------------//
	int INotify::GetFD() const
		{
		return fd_;
		}

	//-----------------------------------------------------------------------------------------//
	void INotify::EnableNonBlocking( bool flag )
		{
		NetUtil::EnableNonBlocking( fd_, flag );
		}
	}
