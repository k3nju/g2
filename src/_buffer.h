#pragma once

#include <vector>
#include <iostream>
#include "debugutility.h"

namespace g2
	{
	using namespace std;

	class Buffer
		{
		public:
			typedef char byte_t;
			
			Buffer();
			explicit Buffer( size_t size );
			Buffer( const byte_t *buffer, size_t size );
			Buffer( const Buffer &rhs );
			const Buffer& operator = ( const Buffer &rhs );
			const Buffer& operator += ( const Buffer &rhs );			
			~Buffer();

			void Write( const byte_t *buffer, size_t size );
			void Write( const void *buffer, size_t sizeInBytes );
			template < class T >
			void Write( const T &val );
			
			size_t Read( byte_t *buffer, size_t size );
			size_t Read( void *buffer, size_t sizeInBytes );
			bool ReadUntil( Buffer &buf, const byte_t *delimiter, size_t delim_size );
			template < class T >
			size_t Read( T &val );

			// returns size. not index
			ssize_t Find( const byte_t *delimiter, size_t delim_size );
			ssize_t Find2( const byte_t *delimiter, size_t delim_size );
			void Extend( size_t size );
			void Allocate( size_t size );
			void Realign();
			void Clear();
			// maybe, Assign is faster than Copy
			void Assign( const byte_t *buffer, size_t size );

			void AddWriteCompletionSize( size_t size );
			void AddReadCompletionSize( size_t size );

			const byte_t* GetBegin() const;
			const byte_t* GetEnd() const;			
			byte_t* GetBegin();
			byte_t* GetEnd();
			const byte_t& operator [] ( unsigned int index ) const;
			byte_t& operator [] ( unsigned int index );

			size_t GetCapacity() const;
			size_t GetSize() const;
			size_t GetFreeSize() const;

		private:
			void Copy( const Buffer &rhs );
			inline void WriteMemory( const byte_t *buffer, size_t size );
			inline void ReadMemory( byte_t *buffer, size_t size );
			
			vector< byte_t > buffer_;
			size_t head_; // bytes from index 0 to start of buffer
			size_t foot_; // bytes from index 0 to end of buffer + 1

		public:
			// for debuggin			
			void EnableDebugging( bool flag )
				{
				isDebugging_ = flag;
				}

			template < class T >
			void Print( T t )
				{
				if( isDebugging_ == true )
					std::cout << t << std::endl;
				}
			
			void Dump( const char *location = NULL )
				{
				if( isDebugging_ == false )
					{
					return;
					}
				
				unsigned char *tmp = (unsigned char*)&buffer_[head_];
				size_t size = foot_ - head_;
				printf( "DEBUG:%s head_=%d foot_=%d size=%d\n",
						location == NULL ? "" : location,
						head_, foot_, size );
				DebugUtil::Dump( tmp, size );
				}

		private:
			bool isDebugging_;
		};

	bool operator == ( const g2::Buffer &lhs, const g2::Buffer &rhs );
	bool operator != ( const g2::Buffer &lhs, const g2::Buffer &rhs );

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void Buffer::Write( const T &val )
		{
		Write( static_cast< const byte_t* >( &val ), sizeof( val ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	size_t Buffer::Read( T &val )
		{
		return Read( static_cast< byte_t* >( &val ), sizeof( val ) );
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__BufferTest

#include <string>
#include <stdio.h>
#include <unistd.h>
#include <memory.h>
#include <vector>
#include <cassert>
#include "debugutility.h"

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devel/gensinx/g2/src/buffer.h	\
/home/kizi/devel/gensinx/g2/src/buffer.cpp
</Makefile.am>
 */

using namespace std;
using namespace g2;

int GutsEntryBufferTest( int, char** )
	{
	/*
	Buffer buf( 1024 );
	int i = 0;
	Buffer::byte_t *p = buf.GetBegin();
	buf.AddWriteCompletionSize( 4 );
	int n = 0xffffffff;
	buf.Write( &n, sizeof( n ) );
	buf.Dump();
	*/

	/*
	const char *msg = "AAA\r\nBBB\r\nCCC\r\n";
	Buffer buf( msg, strlen( msg ) );
	Buffer a;
	assert( buf.ReadUntil( a, "\r\n", 2 ) == true );
	g2::DebugUtil::Dump( a );
	Buffer b;
	assert( buf.ReadUntil( b, "\r\n", 2 ) == true );
	g2::DebugUtil::Dump( b );
	Buffer c;
	assert( buf.ReadUntil( c, "\r\n", 2 ) == true );
	g2::DebugUtil::Dump( c );
	Buffer d;
	assert( buf.ReadUntil( d, "\r\n", 2 ) == false );
	g2::DebugUtil::Dump( d );

	*/

	Buffer buf( "ABCDEFGHIJ", 10 );
	assert( buf.Find2( "A", 1 ) == 1 );
	assert( buf.Find2( "AB", 2 ) == 2 );
	assert( buf.Find2( "ABC", 3 ) == 3 );
	assert( buf.Find2( "B", 1 ) == 2 );
	assert( buf.Find2( "BC", 2 ) == 3 );
	assert( buf.Find2( "BCD", 3 ) == 4 );
	assert( buf.Find2( "FGH", 3 ) == 8 );
	assert( buf.Find2( "J", 1 ) == 10 );
	assert( buf.Find2( "IJ", 2 ) == 10 );
	assert( buf.Find2( "ABCDEFGHIJ", 10 ) == 10 );
	assert( buf.Find2( "AC", 2 ) == -1 );
	assert( buf.Find2( "JK", 2 ) == -1 );
	assert( buf.Find2( "A", 1 ) == buf.Find( "A", 1 ) );
	assert( buf.Find2( "AB", 2 ) == buf.Find( "AB", 2 ) );
	assert( buf.Find2( "ABC", 3 ) == buf.Find( "ABC", 3 ) );
	assert( buf.Find2( "B", 1 ) == buf.Find( "B", 1 ) );
	assert( buf.Find2( "BC", 2 ) == buf.Find( "BC", 2 ) );
	assert( buf.Find2( "BCD", 3 ) == buf.Find( "BCD", 3 ) );
	assert( buf.Find2( "FGH", 3 ) ==buf.Find( "FGH", 3 ) );
	assert( buf.Find2( "J", 1 ) == buf.Find( "J", 1 ) );
	assert( buf.Find2( "IJ", 2 ) == buf.Find( "IJ", 2 ) );
	assert( buf.Find2( "ABCDEFGHIJ", 10 ) ==buf.Find( "ABCDEFGHIJ", 10 ) );
	assert( buf.Find2( "AC", 2 ) == buf.Find( "AC", 2 ) );
	assert( buf.Find2( "JK", 2 ) == buf.Find( "JK", 2 ) );
	buf.Clear();
	buf.Write( "A", 1 );
	assert( buf.Find( "A", 1 ) == buf.Find2( "A", 1 ) );
	assert( buf.Find( "AB", 2 ) == buf.Find2( "AB", 2 ) );
	
	
	return 0;
	}

#endif
#endif
