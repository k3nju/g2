#pragma once
#include <sys/types.h>
#include "uncopyable.h"

namespace g2
	{
	struct FileDescriptor
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

			int fd;
		};

	//-----------------------------------------------------------------------------------------//
	FileDescriptor::FileDescriptor()
		:fd( -1 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	FileDescriptor::~FileDescriptor()
		{
		if( fd != -1 )
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
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Close()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Sync()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void FileDescriptor::Truncate()
		{
		}
	}
