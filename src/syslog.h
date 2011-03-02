#pragma once

#include <sys/syslog.h>
#include <sstream>
#include <iostream>
#include "exception.h"
#include "mutexlock.h"
#include "uncopyable.h"
#include "singletonadaptation.h"

namespace g2
	{
#define G2_INITIALIZE_SYSLOG( ident ) \
	SingSysLog::Instantiate( ident );

#define G2_USE_SYSLOG \
	typedef g2::SingletonAdaptation< g2::SysLog > SingSysLog;

#define G2_EMERGENCY \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_EMERG )
#define G2_ALERT \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_ALERT )
#define G2_CRITICAL \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_CRIT )
#define G2_ERROR \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_ERR )
#define G2_WARNING \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_WARNING )
#define G2_NOTICE \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_NOTICE )
#define G2_INFO \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_INFO )
#define G2_DEBUG \
	g2::ScopedSysLog( SingSysLog::GetInstance(), LOG_DEBUG )
#define G2_END_LOG \
	g2::ScopedSysLog::End();

	
	class SysLog :private g2::Uncopyable< SysLog >
		{
		public:
			SysLog( const char* ident );
			~SysLog();

			void WriteEmerg( const char *msg ); // emergency 
			void WriteAlert( const char *msg );
			void WriteCrit( const char *msg ); // critical
			void WriteError( const char *msg );
			void WriteWarning( const char *msg );
			void WriteInfo( const char *msg ); // information
			void WriteDebug( const char *msg );
		};

	//-----------------------------------------------------------------------------------------//
	class ScopedSysLog
		{
		public:
			struct End {};
			
			ScopedSysLog( SysLog &syslogwr, int level );
			~ScopedSysLog();
			
			template < class T >
			inline ScopedSysLog& operator << ( T msg );
			ScopedSysLog& operator << ( g2::Exception &E );
			inline void operator << ( ScopedSysLog::End );

		private:
			SysLog &SysLog_;
			int lv_;
			bool isFlushed_;
			std::stringstream data_; // thx. stringstream
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ScopedSysLog& ScopedSysLog::operator << ( T msg )
		{
		data_ << msg;

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	void ScopedSysLog::operator << ( ScopedSysLog::End )
		{
		syslog( lv_, "%s", data_.str().c_str() );
		isFlushed_ = true;
		}

	//-----------------------------------------------------------------------------------------//
	// C++ forbids explicit specializetion of member-function in non-namespace.
	// so, i must define it out of class space.
	// ...
	// but, this function don't work well... need to get reference of Exception.
	// but original ref also don't work well.
	// TODO: implement ref function
/*	template<>
	ScopedSysLog& ScopedSysLog::operator << < g2::Exception& > ( g2::Exception &E )
				{
				data_ << "g2::Exception<msg>[" << E.GetMsg().c_str()
					  << "]<number>[" << E.GetNumber()
					  << "]<code>[" << E.GetCode()
					  << "]";

				return *this;
				}
*/
	}



#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SysLogTest

#include <stdio.h>
using namespace g2;

/*
<Makefile.am>
HERE=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(HERE)/syslogwr.h \
$(HERE)/syslogwr.cpp
</Makefile.am>
*/

int GutsEntrySysLogTest( int, char** )
	{
	SysLog Logger( "unit test" );
	ScopedSysLog ScopedLogger( Logger, LOG_DEBUG );
	ScopedLogger << "Test" << ScopedSysLog::End();
	{
	ScopedSysLog ScopedLogger2( Logger, LOG_DEBUG );
	ScopedLogger2 << "Test in scope";
	}
	
	INITIALIZE_SYSLOG( "test" );
	try
		{
		throw Exception( "test exception", 1, 2 );
		}
	catch( Exception &E )
		{
		G2_DEBUG << E << G2_END_LOG;
		}

	return 0;
	}


#endif
#endif
