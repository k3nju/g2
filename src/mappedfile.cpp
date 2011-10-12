#include "mappedfile.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <cassert>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	MappedFile::MappedFile( const std::string &filename, OPEN_FLAGS flags, mode_t mode, off_t pageSize )
		:filename_( filename ),
		 openFlags_( (int)flags ),
		 mode_( mode ),
		 pageSize_( pageSize ),
		 fd_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	MappedFile::~MappedFile()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Open()
		{
		fd_.Open( filename.c_str(), flags_, mode_ );
		mrange.Map( fd_.fd, prot_, Conv2Prot( openFlags_ ), 0, fd_.GetStat().st_size );
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Close()
		{
		mrange.Unmap();
		fd_.Close();
		}

	//-----------------------------------------------------------------------------------------//
	off_t MappedFile::Read( char *buf, off_t size )
		{
		off_t rsize = std::min( size, mrange.GetReadableSize() );
		memmove( buf, mrange.rpos, rsize );
		mrange.AddReadCompletionSize( rsize );

		return rsize;
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Write( const char *buf, off_t size )
		{
		off_t pos = 0;
		while( size > 0 )
			{
			off_t writableSize = mrange.GetWritableSize();
			if( writableSize == 0 )
				{
				off_t n = ( size / pageSize_ ) * pageSize_;
				if( size % pageSize_ > 0 )
					n += pageSize_;

				off_t fileSize = fd_.GetStat().st_size;
				fd_.Truncate( fileSize + n );
				mrange.Remap( 0, fileSize + n );
				}
			
			off_t wsize = std::min( size, mrange.GetWritableSize() );
			memmove( mrange.wpos + pos, buf + pos, wsize );
			mrange.AddWriteCompletionSize( wsize );
			wpos += wsize;
			size -= wsize;
			}
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Flush()
		{
		fd_.Sync();
		}
	}
