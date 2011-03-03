#pragma once

#include <stdio.h>
#include <ctype.h>
#include <map>
#include <string>
#include "utility.h"
#include "mutexlock.h"
#include "uncopyable.h"

namespace g2
	{
#define DebugUtility DebugUtil
#define G2_EOUTPUT( e ) e.Output( __FILE__, __LINE__, __PRETTY_FUNCTION__ );
#define G2_NOW g2::DebugUtil::ScopedDebugPrint< g2::DebugUtil::StdPrint > G2__MARKER__( __PRETTY_FUNCTION__ );
#define G2_DEBUG_PRINT( fmt, ... ) printf( "%s " fmt, g2::Utility::GetTimeString().c_str(), __VA_ARGS__ );

	class Buffer;
	
	class DebugUtility :private g2::Uncopyable< DebugUtility >
		{
		public:
			//-----------------------------------------------------------------------------------------//
			static void Dump( const void *data, size_t size );
			static void Dump( const g2::Buffer &buf );

			//-----------------------------------------------------------------------------------------//
			class ObjectTrace :private g2::Uncopyable< ObjectTrace >
				{
				public:
					static size_t Add( const void *object );
					static size_t Remove( const void *object );
					static size_t GetID( const void *object );

				private:
					typedef std::map< const void*, size_t > object_map_t;
					static size_t sequence_;
					static object_map_t objects_;
					static g2::MutexLock lock_;
				};

			//-----------------------------------------------------------------------------------------//
			class TimeCount :private g2::Uncopyable< TimeCount >
				{
				public:
					TimeCount();
					
					void Start();
					void Stop();
					double Now();
					double GetTime();
					void Output();

					static const char * const FORMAT;// = "%10.30f";

				private:
					double s;
					double e;
				};

			//-----------------------------------------------------------------------------------------//
			template < class Impl >
			class ScopedDebugPrint :private g2::Uncopyable< ScopedDebugPrint< Impl > >
				{
				public:
					ScopedDebugPrint( const char *msg );
					~ScopedDebugPrint();

				private:
					const char *msg_;
				};

			//-----------------------------------------------------------------------------------------//
			// policy class. trivial print
			class StdPrint 
				{
				public:
					static void Print( const std::string &msg );
				};

			//-----------------------------------------------------------------------------------------//
			// policy class. print messages with time prefix
			class StdTimePrint
				{
				public:
					static void Print( const std::string &msg );
				};
			
			//-----------------------------------------------------------------------------------------//
			class BackTrace
				{
				public:
					BackTrace();
					BackTrace( const BackTrace &rhs );
					BackTrace& operator = ( const BackTrace &rhs );
					BackTrace( size_t stackSize );
					~BackTrace();

					void Output() const;
					std::string GetOutputString() const;

				private:
					static const size_t DEFAULT_STACK_SIZE = 100;
					size_t count_;
					void *buffer_[ DEFAULT_STACK_SIZE ];
				};
		};

	//-----------------------------------------------------------------------------------------//
	template < class Impl >
	DebugUtility::ScopedDebugPrint< Impl >::ScopedDebugPrint( const char *msg ):msg_( msg )
		{
		Impl::Print( std::string( "BEGIN :" ) + std::string( msg_ ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Impl >
	DebugUtility::ScopedDebugPrint< Impl >::~ScopedDebugPrint()
		{
		Impl::Print( std::string( "FIN   :" ) + std::string( msg_ ) );
		}			

	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__DebugUtilityTest
#include <string.h>

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/debugutility.h\
$(here)/debugutility.cpp
</Makefile.am>
*/

using namespace g2;

void F()
	{
	DebugUtil::ScopedDebugPrint< DebugUtil::StdPrint > mark( "F()" );
	printf( "some proccessing...\n" );
	}

DebugUtil::BackTrace BT1()
	{
	DebugUtil::BackTrace bt;
	puts( "here" );
	
	return bt;
	}

void Test()
	{
	DebugUtil::BackTrace bt = BT1();
	DebugUtil::BackTrace bt2;
	DebugUtil::BackTrace bt3;

	bt2 = bt;
	puts( "bt2" );
	bt2.Output();

	puts( "bt3" );
	bt3.Output();
	}

int GutsEntryDebugUtilityTest(int, char**)
	{
	//Test();
	G2_DEBUG_PRINT( "test" );
	
	return 0;
	}

#endif
#endif


