#include "serialization.h"
#include <string.h>
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Serialization::Serialization( g2::Buffer &buf )
		:buffer_( &buf )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Serialization::Serialization()
		:buffer_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Serialization::~Serialization()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Serialization::SetBuffer( g2::Buffer &buf )
		{
		buffer_ = &buf;
		}

	//-----------------------------------------------------------------------------------------//
	g2::Buffer& Serialization::GetBuffer()
		{
		assert( buffer_ != NULL && "buffer_ is not set" );

		return *buffer_;
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const st::byte_t &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_BYTE, sizeof( st::ID_BYTE ) );
		buffer_->Write( &value, sizeof( value ) );

		return st::PRIMITIVE_HEADER_SIZE + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const st::word_t &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_WORD, sizeof( st::ID_WORD ) );
		buffer_->Write( &value, sizeof( value ) );
		
		return st::PRIMITIVE_HEADER_SIZE + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const st::dword_t &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_DWORD, sizeof( st::ID_DWORD ) );
		buffer_->Write( &value, sizeof( value ) );
		
		return st::PRIMITIVE_HEADER_SIZE + sizeof( value );
		}
	
	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const st::qword_t &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_QWORD, sizeof( st::ID_QWORD ) );
		buffer_->Write( &value, sizeof( value ) );
		
		return st::PRIMITIVE_HEADER_SIZE + sizeof( value );
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const std::string &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_STRING, sizeof( st::ID_STRING ) );
		st::size_t size = value.size();
		buffer_->Write( &size, sizeof( size ) );
		buffer_->Write( value.c_str(), size );

		return sizeof( st::ID_STRING ) + sizeof( size ) + size;
		}

	//-----------------------------------------------------------------------------------------//
	st::dword_t Serialization::Write( const g2::Buffer &value )
		{
		assert( buffer_ != NULL && "buffer_ is not set" );
		
		buffer_->Write( &st::ID_G2_BUFFER, sizeof( st::ID_G2_BUFFER ) );
		st::size_t size = value.GetSize();
		buffer_->Write( &size, sizeof( size ) );
		*buffer_ += value;

		return sizeof( st::ID_G2_BUFFER ) +	sizeof( size ) + size;
		}
	}
