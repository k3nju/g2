#include "syslog.h"

namespace g2
	{	
	//-----------------------------------------------------------------------------------------//
	SysLog::SysLog( const char *ident )
		{
		openlog( ident, LOG_NDELAY|LOG_PID, LOG_USER );
		}

	//-----------------------------------------------------------------------------------------//
	SysLog::~SysLog()
		{
		closelog();
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteEmerg( const char *msg )
		{
		perror( msg );
		syslog( LOG_EMERG, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteAlert( const char *msg )
		{
		perror( msg );
		syslog( LOG_ALERT, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteCrit( const char *msg )
		{
		perror( msg );
		syslog( LOG_CRIT, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteError( const char *msg )
		{
		perror( msg );
		syslog( LOG_ERR, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteWarning( const char *msg )
		{
		syslog( LOG_WARNING, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteInfo( const char *msg )
		{
		syslog( LOG_INFO, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	void SysLog::WriteDebug( const char *msg )
		{
		syslog( LOG_DEBUG, "%s", msg );
		}

	//-----------------------------------------------------------------------------------------//
	ScopedSysLog::ScopedSysLog( SysLog &syslogwr, int level )
		:SysLog_( syslogwr ), lv_( level ), isFlushed_( false )
		{
		}

	//-----------------------------------------------------------------------------------------//
	ScopedSysLog::~ScopedSysLog()
		{
		if( isFlushed_ == false )
			{
			syslog( lv_, "%s", data_.str().c_str() );
			}
		}

	//-----------------------------------------------------------------------------------------//
	// FIXME: this member-function is workaround
	ScopedSysLog& ScopedSysLog::operator << ( g2::Exception &E )
		{
		data_ << "g2::Exception msg[" << E.GetMsg().c_str()
			  << "] number[" << E.GetNumber()
			  << "] code[" << E.GetCode()
			  << "]";

		return *this;
		}
	}

