#pragma once

#include <cassert>
#include "buffer.h"
#include "uncopyable.h"

namespace g2
	{
	class BinaryData :private g2::Uncopyable
		{
		public:
			BinaryData( g2::Buffer &buf );
			~BinaryData();

			void SetBuffer( g2::Buffer &buf );

			void Pack();
/*			void Pack( const st::char_t &value );
			void Pack( const st::signed_char_t &value );
			void Pack( const st::unsigned_char_t &value );
			void Pack( const st::signed_short_t &value );
			void Pack( const st::unsigned_short_t &value );
			void Pack( const st::signed_int_t &value );
			void Pack( const st::unsigned_int_t &value );
			void Pack( const st::signed_long_t &value );
			void Pack( const st::unsigned_long_t &value );
			void Pack( const st::signed_long_long_t &value );
			void Pack( const st::unsigned_long_long_t &value );
*/
			template < class T >
			void Pack( const T &value );

			bool Unpack();
/*			void Unpack( st::char_t &value );
			void Unpack( st::signed_char_t &value );
			void Unpack( st::unsigned_char_t &value );
			void Unpack( st::signed_short_t &value );
			void Unpack( st::unsigned_short_t &value );
			void Unpack( st::signed_int_t &value );
			void Unpack( st::unsigned_int_t &value );
			void Unpack( st::signed_long_t &value );
			void Unpack( st::unsigned_long_t &value );
			void Unpack( st::signed_long_long_t &value );
			void Unpack( st::unsigned_long_long_t &value );
*/
			template < class T >
			bool Unpack( T &value );

		private:
			g2::Buffer *buffer_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void BinaryData::Pack( const T &value )
		{
		assert( buffer_ != NULL && "buffer_ must not be NULL" );
		buffer_->Write( &value, sizeof( value ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void BinaryData::Unpack( T &value )
		{
		assert( buffer_ != NULL && "buffer_ must not be NULL" );
		if( buffer_->GetSize() < sizeof( T ) )
			{
			return false;
			}

		assert( buffer_->Read( &value, sizeof( value ) ) == sizeof( value ) );

		return true;
		}
	}
