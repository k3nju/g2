#include "debugutility.h"
#include <sys/time.h>
#include <time.h>
#include <execinfo.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>
#include "criticalscope.h"
#include "buffer.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	void DebugUtility::Dump( const void *data, size_t size )
		{
		unsigned char *tmp = (unsigned char*)data;

		size_t offset = 0;
		
		for( size_t i = 0; i < (size/16); ++i )
			{

			for( size_t n = 0; n < 16; ++n )
				{
				printf( "%02x ", tmp[n+offset] );
				}

			printf( ">" );
			for( size_t n = 0; n < 16; ++n )
				{
				if( isprint( tmp[n+offset] ) != 0 )
					{
					printf( "%c", tmp[n+offset] );
					}
				else
					{
					printf( "." );
					}
				}
			printf( "<\n" );

			offset += 16;
			}

		if( (size%16) == 0 )
			{
			return;
			}

		size_t output = 0;
		for( size_t i = 0; i < (size%16); ++i )
			{
			printf( "%02x ", tmp[i+offset]);
			++output;
			}

		for( size_t n = 0; n < (16-output)*3; ++n )
			{
			printf( " " );
			}
		
		printf( ">" );
		for( size_t n = 0; n < (size%16); ++n )
			{
			if( isprint( tmp[offset+n] ) != 0 )
				{
				printf( "%c", tmp[offset+n] );
				}
			else
				{
				printf( "." );
				}
			}
		printf( "<\n" );
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtility::Dump( const g2::Buffer &buf )
		{
		Dump( buf.GetBegin(), buf.GetReadableSize() );
		}

	//-----------------------------------------------------------------------------------------//
	size_t DebugUtility::ObjectTrace::Add( const void *object )
		{
		g2::CriticalScope< g2::MutexLock > locked( lock_ );
		objects_.insert( std::make_pair( object, sequence_ ) );

		return sequence_++;
		}

	//-----------------------------------------------------------------------------------------//
	size_t DebugUtil::ObjectTrace::Remove( const void *object )
		{
		g2::CriticalScope< g2::MutexLock > locked( lock_ );
		size_t id = objects_[ object ];
		object_map_t::iterator iter = objects_.find( object );
		if( iter != objects_.end() )
			{
			objects_.erase( iter );
			}

		return id;
		}

	//-----------------------------------------------------------------------------------------//
	size_t DebugUtil::ObjectTrace::GetID( const void *object )
		{
		g2::CriticalScope< g2::MutexLock > locked( lock_ );

		return objects_[ object ];		
		}

	//-----------------------------------------------------------------------------------------//
	size_t DebugUtil::ObjectTrace::sequence_;

	//-----------------------------------------------------------------------------------------//
	std::map <const void*, size_t > DebugUtil::ObjectTrace::objects_;

	//-----------------------------------------------------------------------------------------//
	g2::MutexLock DebugUtil::ObjectTrace::lock_;

	//-----------------------------------------------------------------------------------------//
	DebugUtil::TimeCount::TimeCount()
		{
		s = e = 0;
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtil::TimeCount::Start()
		{
		s = Now();
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtil::TimeCount::Stop()
		{
		e = Now();
		}

	//-----------------------------------------------------------------------------------------//
	double DebugUtil::TimeCount::Now()
		{
		struct timeval tv;
		gettimeofday( &tv, NULL );
		
		return tv.tv_sec + (double)tv.tv_usec*1e-6;
		}

	//-----------------------------------------------------------------------------------------//
	double DebugUtil::TimeCount::GetTime()
		{
		return e - s;
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtil::TimeCount::Output()
		{
		printf( "%10.30f\n", GetTime() );
		}

	//-----------------------------------------------------------------------------------------//
	const char * const DebugUtil::TimeCount::FORMAT = "%10.30f";

	//-----------------------------------------------------------------------------------------//
	void DebugUtility::StdPrint::Print( const std::string &msg )
		{
		puts( msg.c_str() );
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtility::StdTimePrint::Print( const std::string &msg )
		{
		printf( "%s %s",
				Utility::GetTimeString().c_str(),
				msg.c_str() );
		}

	//-----------------------------------------------------------------------------------------//
	DebugUtility::BackTrace::BackTrace()
		:count_( 0 )
		{
		count_ = backtrace( buffer_, 100 );
		}

	//-----------------------------------------------------------------------------------------//
	DebugUtility::BackTrace::BackTrace( const BackTrace &rhs )
		:count_( 0 )
		{
		count_ = rhs.count_;
		memmove( buffer_, rhs.buffer_, sizeof( buffer_ ) );
		}

	//-----------------------------------------------------------------------------------------//
	DebugUtility::BackTrace& DebugUtility::BackTrace::operator = ( const BackTrace &rhs )
		{
		if( this != &rhs )
			{
			count_ = rhs.count_;
			memmove( buffer_, rhs.buffer_, sizeof( buffer_ ) );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	DebugUtility::BackTrace::~BackTrace()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void DebugUtility::BackTrace::Output() const
		{
		puts( GetOutputString().c_str() );
		}

	//-----------------------------------------------------------------------------------------//
	std::string DebugUtility::BackTrace::GetOutputString() const
		{
		char **strings = backtrace_symbols( buffer_, count_ );
		if( strings == NULL )
			{
			perror( "backtrace_symbols" );
			
			return std::string();
			}

		std::stringstream ss;
		for( size_t i = 0; i < count_-1; ++i )
			{
			ss << strings[i] << "\n";
			}
		ss << strings[count_-1];

		free( strings );

		return ss.str();
		}
	}
