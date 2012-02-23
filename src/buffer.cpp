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
		 data_( NULL ),
		 dataSize_( 0 )
		{
		Allocate( DEFAULT_CAPACITY );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( size_t size )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 data_( NULL ),
		 dataSize_( 0 )
		{
		if( size == 0 )
			{
			size = DEFAULT_CAPACITY;
			}
		
		Allocate( size );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const uint8_t *buf, size_t bufSize )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 data_( NULL ),
		 dataSize_( 0 )
		{
		Allocate( bufSize );
		Write( buf, bufSize );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const Buffer &rhs )
		:chunk_( NULL ),
		 chunkSize_( 0 ),
		 data_( NULL ),
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
		chunk_ = NULL;
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
	size_t Buffer::Write( const uint8_t *bufIn, size_t bufInSize )
		{
		EnsureCapacity( bufInSize );
		memmove( data_ + dataSize_, bufIn, bufInSize );
		dataSize_ += bufInSize;

		return bufInSize;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::Read( uint8_t *bufOut, size_t bufOutSize )
		{
		size_t readSize = std::min( bufOutSize, dataSize_ );
		
		memmove( bufOut, data_, readSize );
		data_ += readSize;
		dataSize_ -= readSize;

		return readSize;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::EnsureCapacity( size_t bufSize )
		{
		size_t remain = GetRemainingSize();
		if( remain >= bufSize )
			{
			return;
			}

		size_t blankSize = data_ - chunk_;
		size_t newSize = chunkSize_ * 2;
		size_t usedSize = ( ( data_ + dataSize_ ) - chunk_ );
		while( bufSize > newSize - usedSize )
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
		data_ = chunk_ + blankSize;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Clear()
		{
		data_ = chunk_;
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
		return ( chunk_ + chunkSize_ ) - ( data_ + dataSize_ );
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddReadCompletionSize( size_t sizeRead )
		{
		data_ += sizeRead;
		dataSize_ -= sizeRead;

		if( (size_t)( data_ - chunk_ ) > chunkSize_ / 2 )
			{
			memmove( chunk_, data_, dataSize_ );
			data_ = chunk_;
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddWriteCompletionSize( size_t sizeWritten )
		{
		dataSize_ += sizeWritten;
		}

	//-----------------------------------------------------------------------------------------//
	const uint8_t* Buffer::GetBegin() const
		{
		return data_;
		}

	//-----------------------------------------------------------------------------------------//
	uint8_t* Buffer::GetEnd()
		{
		return data_ + dataSize_;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Allocate( size_t size )
		{
		assert( chunk_ == NULL );
		assert( chunkSize_ == 0 );

		size = AlignSize( size );
		uint8_t *m = malloc( size );
		if( m == NULL )
			{
			throw std::bad_alloc();
			}

		data_ = chunk_ = static_cast< uint8_t* >( m );
		chunkSize_ = size;
		}
	}
