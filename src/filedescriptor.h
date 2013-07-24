#pragma once
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "exception.h"
#include "uncopyable.h"

namespace g2
	{
	class FileDescriptor
		{
		private:
			G2_MARK_UNCOPYABLE( FileDescriptor );
			
		public:
			inline FileDescriptor();
			inline ~FileDescriptor();
			
			inline void Open( const char *file, int flags, mode_t mode );
			inline void Close();
			inline void Sync();
			inline void Truncate( off_t size );

			inline bool IsClosed() const { return fd_ == -1; }
			inline int GetDescriptor() const { return fd_; }

		private:
			int fd_;
		};

	//-----------------------------------------------------------------------------------------//
	FileDescriptor::FileDescriptor()
		:fd_( -1 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	FileDescriptor::~FileDescriptor()
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
	void FileDescriptor::Open( const char *file, int flags, mode_t mode )
		{
		fd_ = open( file, flags, mode );
		if( fd_ == -1 )
			{
			throw Exception( "open() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Close()
		{
		if( close( fd_ ) == -1 )
			{
			throw Exception( "close() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Sync()
		{
		if( fdatasync( fd_ ) == -1 )
			{
			throw Exception( "fdatasync() failed", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Truncate( off_t length )
		{
		if( ftruncate( fd_, length ) == -1 )
			{
			throw Exception( "ftruncate() failed", errno );
			}
		}
	}
