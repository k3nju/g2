#include "deserialization.h"
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Deserialization::Deserialization( g2::Buffer &buf )
		:buffer_( &buf ),
		 begin_( NULL ),
		 size_( 0 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Deserialization::Deserialization()
		:buffer_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Deserialization::~Deserialization()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Deserialization::SetBuffer( g2::Buffer &buf )
		{
		buffer_ = &buf;
		begin_ = NULL;
		size_ = 0;
		}

	//-----------------------------------------------------------------------------------------//
	g2::Buffer& Deserialization::GetBuffer()
		{
		assert( buffer_ != NULL && "buffer_ is NULL" );
		return *buffer_;
		}

	//-----------------------------------------------------------------------------------------//
	Deserialization::byte_t* Deserialization::GetBegin()
		{
		assert( begin_ != NULL && "begin_ is NULL" );
		return begin_;
		}

	//-----------------------------------------------------------------------------------------//
	st::size_t Deserialization::GetSize()
		{
		return size_;
		}

	//-----------------------------------------------------------------------------------------//
	void Deserialization::AddReadCompletionSize( st::size_t size )
		{
		assert( begin_ != NULL && "begin_ is NULL" );
		begin_ += size;
		size_ -= size;
		}

	//-----------------------------------------------------------------------------------------//
	void Deserialization::InitLocalBuffer()
		{
		assert( buffer_ != NULL && "buffer_ is NULL" );
		begin_ = buffer_->GetBegin();
		size_ = buffer_->GetSize();
		}

	//-----------------------------------------------------------------------------------------//
	void Deserialization::Commit()
		{
		assert( buffer_ != NULL && "buffer_ is NULL" );
		buffer_->AddReadCompletionSize( buffer_->GetSize() - size_ );
		}

	//-----------------------------------------------------------------------------------------//
	bool Deserialization::Read( std::string &value )
		{
		InitLocalBuffer();
		st::dword_t size = ReadImpl( value );
		if( size == 0 )
			{
			return false;
			}

		assert( size == buffer_->GetSize() - size_ );
		Commit();

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	bool Deserialization::Read( g2::Buffer &value )
		{
		InitLocalBuffer();
		st::dword_t size = ReadImpl( value );
		if( size == 0 )
			{
			return false;
			}

		assert( size == buffer_->GetSize() - size_ );		
		Commit();

		return true;
		}
	
	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( st::byte_t &value )
		{
		if( GetSize() < sizeof( st::id_t ) + sizeof( value ) )
			{
			return 0;
			}

		if( *(st::id_t*)GetBegin() != st::ID_BYTE )
			{
			throw st::TypeMismatchException( "ID is not ID_BYTE" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );
		
		value = *(st::byte_t*)GetBegin();
		AddReadCompletionSize( sizeof( value ) );

		return sizeof( st::id_t ) + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( st::word_t &value )
		{
		if( GetSize() < sizeof( st::id_t ) + sizeof( value ) )
			{
			return 0;
			}

		if( *(st::id_t*)GetBegin() != st::ID_WORD )
			{
			throw st::TypeMismatchException( "ID is not ID_WORD" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );
		
		value = *(st::word_t*)GetBegin();
		AddReadCompletionSize( sizeof( value ) );

		return sizeof( st::id_t ) + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( st::dword_t &value )
		{
		if( GetSize() < sizeof( st::id_t ) + sizeof( value ) )
			{
			return 0;
			}
		
		if( *(st::id_t*)GetBegin() != st::ID_DWORD )
			{
			throw st::TypeMismatchException( "ID is not ID_DWORD" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );
		
		value = *(st::dword_t*)GetBegin();
		AddReadCompletionSize( sizeof( value ) );

		return sizeof( st::id_t ) + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( st::qword_t &value )
		{
		if( GetSize() < sizeof( st::id_t ) + sizeof( value ) )
			{
			return 0;
			}
		
		if( *(st::id_t*)GetBegin() != st::ID_QWORD )
			{
			throw st::TypeMismatchException( "ID is not ID_QWORD" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );

		value = *(st::qword_t*)GetBegin();
		AddReadCompletionSize( sizeof( value ) );

		return sizeof( st::id_t ) + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( std::string &value )
		{
		if( GetSize() < st::STRING_HEADER_SIZE )
			{
			return 0;
			}
		
		if( *(st::id_t*)GetBegin() != st::ID_STRING )
			{
			throw st::TypeMismatchException( "Type id is not std::string" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );

		st::size_t size = *(st::size_t*)GetBegin();
		AddReadCompletionSize( sizeof( size ) );

		if( GetSize() < size )
			{
			return 0;
			}

		value.resize( size );
		value.assign( GetBegin(), size );

		AddReadCompletionSize( size );

		return st::STRING_HEADER_SIZE + size;
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Deserialization::ReadImpl( g2::Buffer &value )
		{
		if( GetSize() < st::G2_BUFFER_HEADER_SIZE )
			{
			return 0;
			}

		if( *(st::id_t*)GetBegin() != st::ID_G2_BUFFER )
			{
			throw st::TypeMismatchException( "Type id is not g2::Buffer" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );

		st::size_t size = *(st::size_t*)GetBegin();
		AddReadCompletionSize( sizeof( size ) );

		if( GetSize() < size )
			{
			return 0;
			}

		if( value.GetFreeSize() < size )
			{
			value.Extend( size );
			}

		value.Assign( GetBegin(), size );

		AddReadCompletionSize( size );

		return st::G2_BUFFER_HEADER_SIZE + size;
		}
	}
