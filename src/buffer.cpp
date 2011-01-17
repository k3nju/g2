#include "buffer.h"
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <algorithm>
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer()
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 dataHead_( NULL ),
		 dataSize_( 0 )
		{
		Allocate( DEFAULT_CAPACITY );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( size_t size )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 dataHead_( NULL ),
		 dataSize_( 0 )
		{
		if( size == 0 )
			{
			size = DEFAULT_CAPACITY;
			}
		
		Allocate( size );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const void *buf, size_t bufSize )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 dataHead_( NULL ),
		 dataSize_( 0 )
		{
		Allocate( bufSize );
		Write( buf, bufSize );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const Buffer &rhs )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 dataHead_( NULL ),
		 dataSize_( 0 )
		{
		size_t rSize = rhs.GetReadableSize();
		Allocate( rSize );
		Write( rhs.GetBegin(), rSize );
		}

	//-----------------------------------------------------------------------------------------//
	const Buffer& Buffer::operator = ( const Buffer &rhs )
		{
		if( this != &rhs )
			{
			Clear();
			Write( rhs.GetBegin(), rhs.GetReadableSize() );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	const Buffer& Buffer::operator += ( const Buffer &rhs )
		{
		if( this != &rhs )
			{
			Write( rhs.GetBegin(), rhs.GetReadableSize() );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::~Buffer()
		{
		free( chunk_ );
		}

	//-----------------------------------------------------------------------------------------//
	bool Buffer::operator == ( const Buffer &rhs )
		{
		return GetReadableSize() == rhs.GetReadableSize()
			&& memcmp( GetBegin(), rhs.GetBegin(), rhs.GetReadableSize() ) == 0;
		}

	//-----------------------------------------------------------------------------------------//
	bool Buffer::operator != ( const Buffer &rhs )
		{
		return ! Buffer::operator == ( rhs );
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::Write( const void *bufIn, size_t bufInSize )
		{
		EnsureCapacity( bufInSize );
		memmove( dataHead_ + dataSize_, bufIn, bufInSize );
		dataSize_ += bufInSize;

		return bufInSize;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::Read( void *bufOut, size_t bufOutSize )
		{
		size_t readSize = std::min( bufOutSize, dataSize_ );
		
		memmove( bufOut, dataHead_, readSize );
		dataHead_ += readSize;
		dataSize_ -= readSize;

		return readSize;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::EnsureCapacity( size_t bufSize )
		{
		size_t rest = GetRestSize();
		if( rest >= bufSize )
			{
			return;
			}

		size_t blankSize = dataHead_ - chunk_;
		size_t newSize = chunkSize_ * 2;
		while( bufSize > newSize - rest )
			{
			newSize *= 2;
			}

		void *m = realloc( chunk_, newSize );
		if( m == NULL )
			{
			throw std::bad_alloc();
			}
		
		chunk_ = static_cast< uint8_t* >( m );
		chunkSize_ = newSize;
		dataHead_ = chunk_ + blankSize;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Clear()
		{
		dataHead_ = chunk_;
		dataSize_ = 0;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::GetReadableSize() const
		{
		return dataSize_;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::GetWritableSize() const
		{
		return ( chunk_ + chunkSize_ ) - ( dataHead_ + dataSize_ );
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddReadCompletionSize( size_t sizeRead )
		{
		dataHead_ += sizeRead;
		dataSize_ -= sizeRead;

		if( (size_t)( dataHead_ - chunk_ ) > chunkSize_ / 2 )
			{
			memmove( chunk_, dataHead_, dataSize_ );
			dataHead_ = chunk_;
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddWriteCompletionSize( size_t sizeWritten )
		{
		dataSize_ += sizeWritten;
		}

	//-----------------------------------------------------------------------------------------//
	const void* Buffer::GetBegin() const
		{
		return dataHead_;
		}

	//-----------------------------------------------------------------------------------------//
	void* Buffer::GetEnd()
		{
		return dataHead_ + dataSize_;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Allocate( size_t size )
		{
		assert( chunk_ == NULL );
		assert( chunkSize_ == 0 );

		size = AlignSize( size );
		void *m = malloc( size );
		if( m == NULL )
			{
			throw std::bad_alloc();
			}

		dataHead_ = chunk_ = static_cast< uint8_t* >( m );
		chunkSize_ = size;
		}
	}
