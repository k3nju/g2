#pragma once

#include <sys/types.h>
#include <string>
#include <sstream>
#include "uncopyable.h"

namespace g2
	{
// this may be dangerous
// #define Util Utility
	
	class Utility :private g2::Uncopyable< Utility >
		{
		public:
			template < class T >
			static const std::string Convert2String( T t );
			template < class T >
			static T ConvertHex2( const std::string &hexString );
			static const std::string GetTimeString( const char *format = "%x-%H:%M:%S" );
			static unsigned long GetCRC32( const unsigned char *buf, size_t size );

			static ssize_t Read( int fd, void *buf, size_t count );
			static ssize_t Write( int fd, void *buf, size_t count );
			static int Close( int fd );
		};

	//-----------------------------------------------------------------------------------------//
	// boost::lexical_cast adaptor
	template < class T >
	struct HexTo
		{
			T value;
			operator T() const { return value; }
			
			// this is not member function.
			// this function is helper for implicit type conversion.
			friend std::istream& operator >> ( std::istream &in, HexTo< T > &out )
				{
				in >> std::hex >> out.value;
				
				return in;
				}
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	const std::string Utility::Convert2String( T t )
		{
		return ((std::stringstream&)(std::stringstream() << t)).str();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T Utility::ConvertHex2( const std::string &hexString )
		{
		std::stringstream ss;
		ss << std::hex << hexString;
		T out;
		ss >> out;

		return out;
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__UtilityTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/utility.cpp
</Makefile.am>
*/

#include <stdio.h>
#include <cstring>
using namespace g2;

int GutsEntryUtilityTest(int,char**)
	{
	//int i = 0x80000000;
	//printf("%s\n",  Utility::Convert2String( i ).c_str() );
	//printf( "%s\n", Utility::GetTimeString( "%x-%H:%M:%S" ).c_str() );
	//printf( "%d\n", Utility::ConvertHex2<int>( "0x11" ) );
	//printf( "%p\n", Utility::ConvertHex2<void*>( "0x11" ) );
	const unsigned char *buf = (const unsigned char*)"AB";//AAAAAAAAAAAAAAAAAAAAAAABBBBBBBnoviuoiabshfagowea0897094357034857vsdafkjng0845uasdjfnals";
	unsigned long crc32 = Utility::GetCRC32( buf, 2 );
	printf( "%lx\n", crc32 );

	return 0;
	}

#endif
#endif
