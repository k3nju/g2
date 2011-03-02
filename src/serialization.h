#pragma once

#include <vector>
#include <map>
#include <string>
#include <string.h>
#include "exception.h"
#include "serializationtype.h"
#include "uncopyable.h"
#include "buffer.h"

namespace g2
	{
	namespace st = g2::serialization_type;
	
	class Serialization :private g2::Uncopyable< Serialization >
		{
		public:
			typedef size_t pad_t;
			
			explicit Serialization( g2::Buffer &buf );
			Serialization();
			~Serialization();

			void SetBuffer( g2::Buffer &buf );
			g2::Buffer& GetBuffer();
			
			st::dword_t Write( const st::byte_t &value );
			st::dword_t Write( const st::word_t &value );
			st::dword_t Write( const st::dword_t &value );
			st::dword_t Write( const st::qword_t &value );
			template < class T >
			st::dword_t Write( const T *value, st::size_t size );
			template < class T >
			st::dword_t Write( const T &value );
			st::dword_t Write( const std::string &value );
			st::dword_t Write( const g2::Buffer &value );
			// TODO: need interface for std::set ?

		private:
			g2::Buffer *buffer_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	st::dword_t Serialization::Write( const T *value, st::size_t size )
		{
		// TODO: is this valid for using ID_STRUCT, ok?
		buffer_->Write( &st::ID_STRUCT, sizeof( st::ID_STRUCT ) );
		buffer_->Write( &size, sizeof( size ) );
		buffer_->Write( value, size );
		
		return sizeof( st::ID_STRUCT ) + sizeof( size ) + size;
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class T >
	st::dword_t Serialization::Write( const T &value )
		{
		buffer_->Write( &st::ID_STRUCT, sizeof( st::ID_STRUCT ) );
		st::size_t size = sizeof( value );
		buffer_->Write( &size, sizeof( size ) );
		buffer_->Write( &value, sizeof( T ) );

		return sizeof( st::ID_STRUCT ) + sizeof( size ) + size;
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SerializationTest

#include "debugutility.h"
#include <cassert>
#include <string.h>

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/serialization.h
$(here)/serialization.cpp
</Makefile.am>
*/

using namespace std;
namespace st = g2::serialization_type;

struct Hoge
	{
		int i;
		short s;
	};

int GutsEntrySerializationTest(int,char**)
	{
	g2::Buffer buf( 1 );
	g2::Serialization s( buf );

	st::byte_t b = 0x11;
	s.Write( b );
	st::word_t w = 0x2222;
	s.Write( w );
	st::dword_t d = 0x33333333;
	s.Write( d );
	st::qword_t q = 0x00112233;
	q = ( q << 32 ) | 0x44556677;
	s.Write( q );
	Hoge h0;
	h0.i= 0x55555555;
	h0.s = 0x6666;
	s.Write( h0 );
	Hoge h1;
	h1.i = 0x77777777;
	h1.s = 0x8888;
	s.Write( &h1, sizeof( h1 ) );
	std::string str( "HOGEHOGE" );
	s.Write( str );
	g2::Buffer gbuf;
	gbuf.Write( "AABBCCDD", 8 );
	s.Write( gbuf );
	
	g2::DebugUtil::Dump( buf );
	
	return 0;
	}

#endif
#endif
