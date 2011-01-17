#include "buffer.h"
#include <string.h>
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer()
		:buffer_(),
		 head_( 0 ), foot_( 0 ),
		 isDebugging_( false )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( size_t size ):head_( 0 ), foot_( 0 )
		{
		buffer_.resize( size );
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const Buffer::byte_t *buffer, size_t size ):head_( 0 ), foot_( 0 )
		{
		buffer_.resize( size );
		Write( buffer, size );
		}
	
	//-----------------------------------------------------------------------------------------//
	Buffer::Buffer( const Buffer &rhs )
		{
		Copy( rhs );
		}

	//-----------------------------------------------------------------------------------------//
	const Buffer& Buffer::operator = ( const Buffer &rhs )
		{
		if( this != &rhs )
			{
			Copy( rhs );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	const Buffer& Buffer::operator += ( const Buffer &rhs )
		{
		if( &rhs != this )
			{
			Write( rhs.GetBegin(), rhs.GetSize() );
			}

		return *this;
		}	

	//-----------------------------------------------------------------------------------------//
	Buffer::~Buffer()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Write( const byte_t* buffer, size_t size )
		{
		if( size == 0 )
			{
			return;
			}

		if( GetFreeSize() < size )
			{
			Extend( size * 2 );
			}

		WriteMemory( buffer, size );
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Write( const void *buffer, size_t sizeInBytes )
		{
		Write( static_cast< const byte_t* >( buffer ), sizeInBytes );
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::Read( byte_t *buffer, size_t size )
		{
		if( size == 0 || GetSize() == 0 )
			{
			return 0;
			}
		
		size_t readSize = size > GetSize() ? GetSize() : size;
		ReadMemory( buffer, readSize );

		return readSize;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::Read( void *buffer, size_t sizeInBytes )
		{
		return Read( static_cast< byte_t* >( buffer ), sizeInBytes );
		}

	//-----------------------------------------------------------------------------------------//
	bool Buffer::ReadUntil( Buffer &buf, const byte_t *delimiter, size_t delimSize )
		{
		ssize_t size = Find( delimiter, delimSize );
		if( size == -1 )
			{
			return false;
			}
		
		buf.Extend( size );
		
		assert( buf.GetFreeSize() >= (size_t)size );
		
		ReadMemory( buf.GetEnd(), buf.GetFreeSize() );
		buf.AddWriteCompletionSize( size );
		
		assert( buf.GetSize() >= (size_t)size );

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t Buffer::Find( const byte_t *delimiter, size_t delimSize )
		{
		if( delimSize == 0 || foot_ - head_ < delimSize - 1 )
			{
			return -1;
			}

		size_t loopCount = ( foot_ - head_ ) - ( delimSize - 1 );
		size_t i = 0;

		while( i < loopCount )
			{
			size_t n = 0;
			while( n < delimSize )
				{
				if( buffer_[ head_ + i + n ] != delimiter[n] )
					{
					break;
					}
				++n;
				}

			if( n == delimSize )
				{
				return i + n;
				}

			++i;
			}

		return -1;
		}

	//-----------------------------------------------------------------------------------------//
	ssize_t Buffer::Find2( const byte_t *delimiter, size_t delimSize )
		{
		if( delimSize == 0 || foot_ - head_ < delimSize )
			{
			return -1;
			}

		size_t size = foot_ - head_;
		byte_t *find = (byte_t*)memchr( &buffer_[head_], *delimiter, size );
		
		if( find == NULL )
			{
			return -1;
			}

		size = &buffer_[foot_] - find;
		if( size < delimSize )
			{
			return -1;
			}

		if( memcmp( find, delimiter, delimSize ) != 0 )
			{
			return -1;
			}

		return ( find - &buffer_[head_] ) + delimSize;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Extend( size_t size )
		{
		Realign();
		buffer_.resize( buffer_.size() + size );
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Allocate( size_t size )
		{
		Realign();
		if( GetFreeSize() < size )
			{
			Extend( size - GetFreeSize() );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Realign()
		{
		if( head_ > 0 )
			{
			memmove( &(buffer_[0]), &(buffer_[head_]), foot_ - head_ );
			foot_ -= head_;
			head_ = 0;
			}
		}	

	//-----------------------------------------------------------------------------------------//
	void Buffer::Clear()
		{
		head_ = foot_ = 0;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::Assign( const byte_t *buffer, size_t size )
		{
		Clear();
		Write( buffer, size );
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddWriteCompletionSize( size_t size )
		{
		foot_ += size;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::AddReadCompletionSize( size_t size )
		{
		head_ += size;
		if( head_ == foot_ )
			{
			head_ = foot_ = 0;
			}
		}
		
	//-----------------------------------------------------------------------------------------//
	const Buffer::byte_t* Buffer::GetBegin() const
		{
		return &buffer_[ head_ ];
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::byte_t* Buffer::GetBegin()
		{
		return const_cast< byte_t* >( static_cast< const Buffer* >( this )->GetBegin() );
		}
	
	//-----------------------------------------------------------------------------------------//
	const Buffer::byte_t* Buffer::GetEnd() const
		{
		return &buffer_[ foot_ ];
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::byte_t* Buffer::GetEnd()
		{
		return const_cast< byte_t* >( static_cast< const Buffer* >( this )->GetEnd() );
		}

	//-----------------------------------------------------------------------------------------//
	const Buffer::byte_t& Buffer::operator [] ( unsigned int index ) const
		{
		return buffer_[index];
		}

	//-----------------------------------------------------------------------------------------//
	Buffer::byte_t& Buffer::operator [] ( unsigned int index )
		{
		return const_cast< byte_t& >( static_cast< const Buffer* >( this )->operator []( index ) );
		}
		
	//-----------------------------------------------------------------------------------------//
	size_t Buffer::GetCapacity() const
		{
		return buffer_.capacity();
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::GetSize() const
		{
		return foot_ - head_;
		}

	//-----------------------------------------------------------------------------------------//
	size_t Buffer::GetFreeSize() const
		{
		if( buffer_.empty() == true )
			{
			return 0;
			}
		
		return buffer_.size() - foot_;
		}


	//-----------------------------------------------------------------------------------------//
	void Buffer::Copy( const Buffer &rhs )
		{
		buffer_ = rhs.buffer_;
		head_ = rhs.head_;
		foot_ = rhs.foot_;
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::WriteMemory( const Buffer::byte_t *buffer, size_t size )
		{
		memmove( &buffer_[foot_] , buffer, size );
		foot_ += size;		
		}

	//-----------------------------------------------------------------------------------------//
	void Buffer::ReadMemory( Buffer::byte_t *buffer, size_t size )
		{
		memmove( buffer, &buffer_[head_], size );
		head_ += size;
		}

	//-----------------------------------------------------------------------------------------//
	bool operator == ( const g2::Buffer &lhs, const g2::Buffer &rhs )
		{
		return lhs.GetSize() == rhs.GetSize() && memcmp( lhs.GetBegin(), rhs.GetBegin(), lhs.GetSize() ) == 0;
		}

	//-----------------------------------------------------------------------------------------//
	bool operator != ( const g2::Buffer &lhs, const g2::Buffer rhs )
		{
		return ! operator == ( lhs, rhs );
		}
	}
