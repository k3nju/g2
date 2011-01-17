#pragma once

#include <vector>
#include <map>
#include <string>
#include <deque>
#include <cassert>
#include "exception.h"
#include "serializationtype.h"
#include "uncopyable.h"
#include "buffer.h"

namespace g2
	{
	namespace st = g2::serialization_type;
	
	class Deserialization :private g2::Uncopyable
		{
		public:
			explicit Deserialization( g2::Buffer &buf );
			Deserialization();
			~Deserialization();

			void SetBuffer( g2::Buffer &buf );
			g2::Buffer& GetBuffer();

/*			bool Read( st::byte_t &value );
			bool Read( st::word_t &value );
			bool Read( st::dword_t &value );
			bool Read( st::qword_t &value );
*/
			template < class T >
			bool Read( T *value, st::size_t size );
			template < class T >
			bool Read( T &value );

/*
			template < class T, size_t N >
			bool Read( T (&value)[N] );
			template < class T >
			bool Read( T *value, st::word_t size );
			template < class T >
			bool Read( std::vector< T > &value );
			template < class K, class V >
			bool Read( std::map< K, V > &value );
*/
			bool Read( std::string &value );
			bool Read( g2::Buffer &value );

		private:
			st::dword_t ReadImpl( st::byte_t &value );
			st::dword_t ReadImpl( st::word_t &value );
			st::dword_t ReadImpl( st::dword_t &value );
			st::dword_t ReadImpl( st::qword_t &value );
			template < class T >
			st::dword_t ReadImpl( T *value, st::size_t size );
			template < class T >
			st::dword_t ReadImpl( T &value );
/*
			template < class T, size_t N >
			st::dword_t ReadImpl( T (&value)[N] );
			template < class T >
			st::dword_t ReadImpl( T *value, st::word_t size );
			template < class T >
			st::dword_t ReadImpl( std::vector< T > &value );
			template < class K, class V >
			st::dword_t ReadImpl( std::map< K, V > &value );
*/
			st::dword_t ReadImpl( std::string &value );
			st::dword_t ReadImpl( g2::Buffer &value );

			typedef g2::Buffer::byte_t byte_t;
			byte_t* GetBegin();
			st::size_t GetSize();
			void AddReadCompletionSize( size_t size );
			void InitLocalBuffer();
			void Commit();

			// TODO:capsule these members
			g2::Buffer *buffer_;
			byte_t *begin_;
			size_t size_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	bool Deserialization::Read( T *value, st::size_t size )
		{
		InitLocalBuffer();
		st::dword_t sizeRead = ReadImpl( value, size );
		if( sizeRead == 0 )
			{
			return false;
			}

		assert( sizeRead == buffer_->GetSize() - size_ );
		Commit();

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	bool Deserialization::Read( T &value )
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
	template < class T >
	st::dword_t Deserialization::ReadImpl( T *value, st::size_t size )
		{
		// is this valid for using STRUCT for UNKNOWN value ok?
		if( GetSize() < st::STRUCT_HEADER_SIZE )
			{
			return 0;
			}

		if( *(st::id_t*)GetBegin() != st::ID_STRUCT )
			{
			throw st::TypeMismatchException( "ID is not ID_STRUCT" );
			}

		AddReadCompletionSize( sizeof( st::id_t ) );

		st::size_t sizePacked = *(st::size_t*)GetBegin();
		if( sizePacked != size )
			{
			throw st::TypeMismatchException( "struct size is not same" );
			}
		AddReadCompletionSize( sizeof( size ) );

		if( GetSize() < size )
			{
			return 0;
			}

		memmove( value, GetBegin(), size );
		AddReadCompletionSize( size );

		return sizeof( st::id_t ) + sizeof( st::word_t ) + size;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	st::dword_t Deserialization::ReadImpl( T &value )
		{
		if( GetSize() < st::STRUCT_HEADER_SIZE )
			{
			return 0;
			}
		
		if( *(st::id_t*)GetBegin() != st::ID_STRUCT )
			{
			throw st::TypeMismatchException( "ID is not ID_STRUCT" );
			}
		AddReadCompletionSize( sizeof( st::id_t ) );

		st::size_t size = *(st::size_t*)GetBegin();
		if( size != sizeof( value ) )
			{
			throw st::TypeMismatchException( "struct size is not same" );
			}
		AddReadCompletionSize( sizeof( size ) );

		if( GetSize() < size )
			{
			return 0;
			}

		memmove( &value, GetBegin(), sizeof( T ) );
		AddReadCompletionSize( sizeof( value ) );

		return sizeof( st::id_t ) + sizeof( size ) + size;
		}
/*
	//-----------------------------------------------------------------------------------------//
	template < class T, size_t N >
	st::dword_t Deserialization::ReadImpl( T (&value)[N] )
		{
		if( buffer_->GetSize() < sizeof( st::ID_FIXED_ARRAY ) )
			{
			return 0;
			}

		g2::Buffer::byte_t * p = buffer_->GetBegin();
		st::id_t id = *(st::id_t*)p;
		p += sizeof( st::id_t );
		if( id != st::ID_FIXED_ARRAY )
			{
			throw st::TypeMismatchException( "Type is not ID_FIXED_ARRAY" );
			}

		st::id_t elemId = st::type_pair< T >::ID;
		if( elemId & st::ID_PRMITIVE )
			{
			st::id_t receivedElemId = *(st::id_t*)p;
			if( receivedElemId != elemId )
				{
				throw st::TypeMismatchException( "Element type is not same" );
				}

			p += sizeof( st::ID_FIXED_ARRAY ) + sizeof( receivedElemId );
			
			st::word_t size = *(st::word_t*)p;
			p += sizeof( size );
			if( size != sizeof( T ) )
				{
				throw st::TypeMismatchException( "Element size is not same" );
				}
			
			st::word_t count = *(st::word_t*)p;
			p += sizeof( count );
			if( count != N )
				{
				throw st::TypeMismatchException( "Fixed array count is not same" );
				}
			
			size_t restSize = buffer_->GetSize() -
				( sizeof( st::ID_FIXED_ARRAY ) +
				  sizeof( receivedElemId ) +
				  sizeof( st::word_t ) +
				  sizeof( st::word_t ) );
				
			if( restSize < ( size * count ) )
				{
				return 0;
				}
			
			memmove( value, p, size * count );

			buffer_->AddReadCompletionSize( sizeof( st::ID_FIXED_ARRAY ) +
											sizeof( receivedElemId ) +
											sizeof( size ) +
											sizeof( count )  +
											( size * count ) );

			return size * count;
			}

		st::word_t count = *(st::count*)p;
		p += sizeof( count );

		st::dword_t total = 0;
		for( st::word_t i = 0; i < count; ++i )
			{
			total += Deserialize( value[i] );
			}

		return total;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	st::dword_t Deserialization::ReadImpl( T *value, st::dword_t size )
		{
		if( buffer_->GetSize() < sizeof( st::ID_ARRAY ) )
			{
			return 0;
			}
		}
*/
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__DeserializationTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/serialization.h\
$(here)/serialization.cpp\
$(here)/deserialization.h\
$(here)/deserialization.cpp
</Makefile.am>
*/

#include <stdio.h>
#include <cassert>
#include <string.h>
#include <string>
#include "serialization.h"
#include "debugutility.h"
#include "buffer.h"

using namespace std;
using namespace g2;
using namespace g2::serialization_type;

void Debug( g2::Buffer &buf )
	{
	g2::DebugUtil::Dump( buf );
	}


struct Hoge
	{
		int i;
		short s;
	};

int GutsEntryDeserializationTest(int,char**)
	{

	Buffer buf( 1024 );
	Serialization s( buf );
	Deserialization ds( buf );

	// basic usage test
	{
	st::byte_t b = 0x11;
	assert( s.Write( b ) == 2 );
	b = 0;
	assert( ds.Read( b ) == true );
	assert( b == 0x11 );
	buf.Clear();
	}

	{
	st::word_t w = 0x2222;
	assert( s.Write( w ) == 3 );
	w = 0;
	assert( ds.Read( w ) == true );
	assert( w == 0x2222 );
	buf.Clear();
	}
	
	{
	st::dword_t d = 0x33333333;
	assert( s.Write( d ) == 5 );
	d = 0;
	assert( ds.Read( d ) == true );
	assert( d == 0x33333333 );
	buf.Clear();
	}

	{
	st::qword_t q = 0x44444444;
	q = q << 32 | 0x55555555;
	assert( s.Write( q ) == 1 + sizeof( q ) );
	q = 0x0;
	assert( ds.Read( q ) == true );
	assert( ( q & 0xffffffff ) == 0x55555555 );
	assert( ( q >> 32 ) == 0x44444444 );
	buf.Clear();
	}


	{
	Hoge t;
	memset( &t, 0, sizeof( t ) );
	t.i = 0x66666666;
	t.s = 0x7777;
	
	assert( s.Write( t ) == st::STRUCT_HEADER_SIZE + sizeof( t ) );
	memset( &t, 0, sizeof( t ) );
	assert( ds.Read( t ) == true );
	assert( t.i == 0x66666666 && t.s == 0x7777 );	
	buf.Clear();
	}

	{
	string str( "HOGEHOGE" );
	assert( s.Write( str ) == st::STRING_HEADER_SIZE + str.size() );
	str.clear();
	assert( ds.Read( str ) == true );
	assert( str == string( "HOGEHOGE" ) );
	buf.Clear();
	}

	{
	const char *p = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
	Buffer tmp;
	tmp.Write( p, strlen( p ) );
	assert( s.Write( tmp ) == st::G2_BUFFER_HEADER_SIZE + strlen( p ) );
	Buffer tmp2;
	assert( ds.Read( tmp2 ) == true );
	assert( memcmp( tmp2.GetBegin(), p, strlen( p ) ) == 0 );
	buf.Clear();
	}


	// size and serialization/deserialization integrity test
	{
	st::byte_t a = 0x11;
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "write size is inconsistent" );
	st::byte_t b = 0;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsistent" );
	
	buf.Clear();
	}

	{
	st::word_t a = 0x2222;
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "write size is inconsistent" );
	st::word_t b = 0;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsisntent" );
	
	buf.Clear();
	}
	
	{
	st::dword_t a = 0x33333333;
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "write size is inconsistent" );
	st::dword_t b = 0;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsistent" );
	
	buf.Clear();
	}

	{
	st::qword_t a = 0x44444444;
	a = a << 32 | 0x55555555;
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "write size is inconsistent" );
	st::qword_t b = 0L;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsinstent" );
	
	buf.Clear();
	}

	{
	Hoge t;
	memset( &t, 0, sizeof( t ) );
	t.i = 0x66666666;
	t.s = 0x7777;
	st::size_t l = s.Write( t );
	assert( l == buf.GetSize() && "write size is inconsistent" );
	Hoge y;
	memset( &y, 0, sizeof( t ) );
	ds.Read( y );
	assert( y.i == t.i && y.s == t.s && "data integrity is mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsistent" );
	
	buf.Clear();
	}

	{
	string a( "HOGEHOGE" );
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "write size is inconsistent" );
	string b;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsistent" );
	
	buf.Clear();
	}

	{
	const char *p = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAB";
	Buffer a;
	a.Write( p, strlen( p ) );
	st::size_t l = s.Write( a );
	assert( buf.GetSize() == l && "wirte size is inconsistent" );
	Buffer b;
	ds.Read( b );
	assert( a == b && "data integrity mismatch" );
	assert( buf.GetSize() == 0 && "read size is inconsistent" );
	
	buf.Clear();
	}

	// boundary test
	{
	st::byte_t a = 0;
	st::byte_t b = 0;	
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );
		}

	for( st::size_t i = l; i < 10; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == tl - l );
		}

	buf.Clear();
	}

	{
	st::word_t a = 0;
	st::word_t b = 0;	
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < 10; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();	
	}

	{
	st::dword_t a = 0;
	st::dword_t b = 0;	
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < 20; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();
	}

	{
	st::qword_t a = 0;
	st::qword_t b = 0;	
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < 20; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();
	}

	{
	Hoge a = {0};
	Hoge b = {0};	
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < l*2; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();
	}

	{
	std::string a( "AAAAAAAAAAAB" );
	std::string b;
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < l*2; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();
	}
	
	{
	Buffer a;
	const char *p = "AAAAAAAAAAAAAAB" ;
	a.Write( p, strlen( p ) );
	Buffer b;
	st::size_t l = s.Write( a );

	for( st::size_t i = 0; i < l-1; ++i )
		{
		Buffer tmp;
		tmp.Write( buf.GetBegin(), i );
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == false );
		assert( tmp.GetSize() == tl );		
		}
	for( st::size_t i = l; i < l*2; ++i )
		{
		Buffer tmp( buf );
		for( st::size_t n = 0; n < i; ++n )
			{
			tmp.Write( &i, sizeof( i ) );
			}
		st::size_t tl = tmp.GetSize();		
		ds.SetBuffer( tmp );
		assert( ds.Read( b ) == true );
		assert( tmp.GetSize() == ( tl - l ) );		
		}

	buf.Clear();
	}



	// zero length test
	{
	buf.Clear();
	s.SetBuffer( buf );
	ds.SetBuffer( buf );
	std::string str;
	s.Write( str );
	std::string str2;
	ds.Read( str2 );
	assert( str == str2 );
	}

	{
	Buffer tmp;
	s.Write( tmp );
	Buffer tmp2;
	ds.Read( tmp2 );
	assert( tmp == tmp2 );
	}

	return 0;
	}
  

#endif
#endif

