p#pragma once
#include <sys/types.h>
#include <stdint.h>
#include <cstdlib>

namespace g2
	{
	class Buffer
		{
		public:
			Buffer();
			explicit Buffer( size_t size );
			Buffer( const uint8_t *buf, size_t size );
			Buffer( const Buffer &rhs );
			const Buffer& operator = ( const Buffer &rhs );
			const Buffer& operator += ( const Buffer &rhs );
			~Buffer();

			bool operator ==( const Buffer &rhs );
			bool operator !=( const Buffer &rhs );

			size_t Write( const uint8_t *buf, size_t size );
			size_t Read( uint8_t *buf, size_t size );

			void EnsureCapacity( size_t size );
			void Clear();
			
			size_t GetReadableSize() const;
			size_t GetWritableSize() const;

			void AddReadCompletionSize( size_t size );
			void AddWriteCompletionSize( size_t size );

			const uint8_t* GetBegin() const;
			uint8_t* GetEnd();
			
			// needs "const void* GetEnd()"?
			// needs "void* GetBegin()"?

		private:
			void Allocate( size_t size );
			size_t AlignSize( size_t size ) const { return ( size + ( ALIGN_SIZE - 1 ) ) & ~( ALIGN_SIZE - 1 ); };
			size_t GetRemainingSize() const { return ( chunk_ + chunkSize_ ) - ( data_ + dataSize_ ); }
			
			uint8_t *chunk_;
			size_t chunkSize_;
			uint8_t *data_;
			size_t dataSize_;
			
			static const size_t ALIGN_SIZE = sizeof( int );
			static const size_t DEFAULT_CAPACITY = 1024 * 4;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__BufferTest

#include <stdio.h>
#include <string.h>
#include <gtest/gtest.h>


int GutsEntryBufferTest(int,char**)
	{
	using namespace g2;
	Buffer b;

	{
	Buffer b;
	EXPECT_TRUE( b.GetWritableSize() > 0 );
	}

	{
	Buffer b( 1024 );
	EXPECT_TRUE( b.GetWritableSize() == 1024 );
	Buffer b2( 1023 );
	EXPECT_TRUE( b.GetWritableSize() == 1024 );
	Buffer b3( 0 );
	EXPECT_TRUE( b.GetWritableSize() != 0 );
	}

	{
	Buffer b( "ABCDEF", 6 );
	EXPECT_TRUE( b.GetReadableSize() == 6 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "ABCDEF", 6 ) == 0 );
	}

	{
	Buffer b( "ABCDEF", 6 );
	Buffer b2( b );
	EXPECT_TRUE( b == b2 );
	EXPECT_TRUE( !( b != b2 ) );
	}

	{
	Buffer b( "A", 1 );
	EXPECT_TRUE( b.Write( "B", 1 ) == 1 );
	EXPECT_TRUE( b.Write( "C", 1 ) == 1 );
	EXPECT_TRUE( b.GetReadableSize() == 3 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "ABC", 3 ) == 0 );
	}

	{
	Buffer b( "ABCDEF", 6 );
	char tmp[10] = {0};
	EXPECT_TRUE( b.Read( tmp, 3 ) == 3 );
	EXPECT_TRUE( memcmp( tmp, "ABC", 3 ) == 0 );
	EXPECT_TRUE( b.Read( tmp, 3 ) == 3 );
	EXPECT_TRUE( memcmp( tmp, "DEF", 3 ) == 0 );
	EXPECT_TRUE( b.Read( tmp, 10 ) == 0 );
	}

	{
	Buffer b( 4 );
	EXPECT_TRUE( b.GetWritableSize() == 4 );
	EXPECT_TRUE( b.GetReadableSize() == 0 );
	EXPECT_TRUE( b.Write( "A", 1 ) == 1 );
	EXPECT_TRUE( b.GetWritableSize() == 3 );
	EXPECT_TRUE( b.GetReadableSize() == 1 );
	EXPECT_TRUE( b.Write( "A", 1 ) == 1 );
	EXPECT_TRUE( b.GetWritableSize() == 2 );
	EXPECT_TRUE( b.GetReadableSize() == 2 );
	EXPECT_TRUE( b.Write( "A", 1 ) == 1 );
	EXPECT_TRUE( b.GetWritableSize() == 1 );
	EXPECT_TRUE( b.GetReadableSize() == 3 );
	EXPECT_TRUE( b.Write( "A", 1 ) == 1 );
	EXPECT_TRUE( b.GetWritableSize() == 0 );
	EXPECT_TRUE( b.GetReadableSize() == 4 );
	EXPECT_TRUE( b.Write( "A", 1 ) == 1 );
	EXPECT_TRUE( b.GetWritableSize() > 0 );
	EXPECT_TRUE( b.GetReadableSize() == 5 );
	}

	{
	Buffer b( "ABCDEF", 6 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "ABCDEF", b.GetReadableSize() ) == 0 );
	b.AddReadCompletionSize( 1 );
	EXPECT_TRUE( b.GetReadableSize() == 5 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "BCDEF", 5 ) == 0 );
	b.AddReadCompletionSize( 5 );
	EXPECT_TRUE( b.GetReadableSize() == 0 );

	b.EnsureCapacity( 6 );
	EXPECT_TRUE( b.GetWritableSize() >= 6 );
	memmove( b.GetEnd(), "GHIJKL", 6 );
	b.AddWriteCompletionSize( 6 );
	EXPECT_TRUE( b.GetReadableSize() == 6 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "GHIJKL", 6 ) == 0 );
	b.EnsureCapacity( 6 );
	EXPECT_TRUE( b.GetWritableSize() >= 6 );
	memmove( b.GetEnd(), "MNOPQR", 6 );
	b.AddWriteCompletionSize( 6 );
	EXPECT_TRUE( b.GetReadableSize() == 12 );
	EXPECT_TRUE( memcmp( b.GetBegin(), "GHIJKLMNOPQR", 12 ) == 0 );
	}

	{
	Buffer b( "ABCDEFGH", 8 );
	printf( "%d\n", b.GetWritableSize() );
	b.AddReadCompletionSize( 5 );
	printf( "%d\n", b.GetWritableSize() );
	EXPECT_TRUE( b.GetWritableSize() >= 5 );
	}
	
	return 0;
	}

#endif
#endif
