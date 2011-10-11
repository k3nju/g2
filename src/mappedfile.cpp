#include "mappedfile.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <errno.h>
#include <algorithm>
#include <cstring>
#include <cassert>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	MappedFile::MappedFile()
		:fd_( -1 ),
		 base_( (char*)MAP_FAILED ),
		 end_( NULL ),
		 pos_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	MappedFile::~MappedFile()
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
	void MappedFile::Open( const char *filename, OPEN_FLAGS flags, mode_t mode, off_t mapSize)
		{
		 fd_ = open( filename, (int)flags, mode );
		if( fd_ == -1 )
			throw Exception( "open() failed", errno );

		if( ftruncate( fd_, mapSize ) == -1 )
			throw Exception( "ftruncate() failed", errno );
		
		int prot = 0;
		if( flags & O_RDONLY ) prot |= PROT_READ;
		if( flags & O_WRONLY ) prot |= PROT_WRITE;
		if( flags & O_RDWR ) prot |= PROT_READ | PROT_WRITE;
		
		base_ = (char*)mmap( NULL, mapSize, prot, MAP_SHARED, fd_, 0 );
		if( base_ == MAP_FAILED )
			{
			throw Exception( "mmap() failed", errno );
			}
		
		end_ = base_ + mapSize;
		pos_ = base_;
		}

	//-----------------------------------------------------------------------------------------//
	size_t MappedFile::Read( char *buf, size_t size )
		{
		if( pos_ >= end_ )
			return 0;

		size_t rsize = std::min( size, (size_t)( end_ - pos_ ) );
		memmove( buf, pos_, rsize );
		pos_ += rsize;
		assert( pos_ <= end_ );
		
		return rsize;
		}

	//-----------------------------------------------------------------------------------------//
	size_t MappedFile::Write( const char *buf, size_t size )
		{
		if( pos_ >= end_ )
			return 0;

		size_t wsize = std::min( size, (size_t)( end_ - pos_ ) );
		memmove( pos_, buf, wsize );
		pos_ += wsize;
		assert( pos_ <= end_ );

		return wsize;
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Close()
		{
		if( munmap( base_, end_ - base_ ) == -1 )
			{
			throw Exception( "munmap() failed", errno );
			}
		base_ = (char*)MAP_FAILED;
		
		if( fd_ != -1 )
			{
			if( close( fd_ ) == -1 )
				{
				throw Exception( "close() failed", errno );
				}
			
			fd_ = -1;
			}
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Flush()
		{
		// TODO: should be msync?
		if( fdatasync( fd_ ) == -1 )
			throw Exception( "fdatasync() failed", errno );
		}

	//-----------------------------------------------------------------------------------------//
	off_t MappedFile::SetOffset( off_t offset )
		{
		off_t off = std::min( offset, (off_t)( end_ - base_ ) );
		pos_ = base_ + off;

		return off;
		}

	//-----------------------------------------------------------------------------------------//
	off_t MappedFile::GetOffset() const
		{
		return pos_ - base_;
		}

	//-----------------------------------------------------------------------------------------//
	size_t MappedFile::Forward( size_t size )
		{
		size_t s = std::min( size, (size_t)( end_ - pos_ ) );
		pos_ += s;
		assert( pos_ <= end_ );
		
		return s;
		}

	//-----------------------------------------------------------------------------------------//
	size_t MappedFile::Backward( size_t size )
		{
		size_t s = std::min( size, (size_t)( pos_ - base_ ) );
		pos_ -= s;
		assert( pos_ >= base_ );

		return s;
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Extend( size_t size )
		{
		off_t currentSize = end_ - base_;
		off_t newSize = currentSize + size;
		if( ftruncate( fd_, newSize ) == -1 )
			throw Exception( "ftruncate() failed", errno );

		void *newBase = mremap( base_, currentSize, newSize, MREMAP_MAYMOVE );
		if( newBase == MAP_FAILED )
			throw Exception( "mremap() failed", errno );

		size_t posOffset = pos_ - base_;
		base_ = (char*)newBase;
		end_ = base_ + newSize;
		pos_ = base_ + posOffset;
		}
	}
