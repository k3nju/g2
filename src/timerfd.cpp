#include "timerfd.h"
#include <errno.h>
#include "exception.h"
#include "utility.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	TimerFD::TimerFD()
		:fd_( -1 )
		{
		fd_ = timerfd_create( CLOCK_REALTIME, 0 );
		if( fd_ == -1 )
			{
			throw Exception( "timerfd_create() failed()", errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	TimerFD::~TimerFD()
		{
		try
			{
			Close();
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::Start( time_t initSec, long initNsec, time_t intrvSec, long intrvNsec )
		{
		struct timespec now;
		clock_gettime( CLOCK_REALTIME, &now );

		struct itimerspec spec;
		spec.it_value.tv_sec = now.tv_sec + initSec;
		spec.it_value.tv_nsec = now.tv_nsec + initNsec;	
		spec.it_interval.tv_sec = intrvSec;
		spec.it_interval.tv_nsec = intrvNsec;

		SetTime( spec );
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::StartOneShot( time_t sec, long nsec )
		{
		struct timespec now;
		clock_gettime( CLOCK_REALTIME, &now );

		struct itimerspec spec;
		spec.it_value.tv_sec = now.tv_sec + sec;
		spec.it_value.tv_nsec = now.tv_nsec + nsec;
		spec.it_interval.tv_sec = 0;
		spec.it_interval.tv_nsec = 0;

		SetTime( spec );
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::StartInterval( time_t sec, long nsec )
		{
		struct timespec now;
		clock_gettime( CLOCK_REALTIME, &now );

		struct itimerspec spec;
		spec.it_value.tv_sec = now.tv_sec + sec;
		spec.it_value.tv_nsec = now.tv_nsec + nsec;
		spec.it_interval.tv_sec = sec; 
		spec.it_interval.tv_nsec = nsec;

		SetTime( spec );
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::Stop()
		{
		struct itimerspec spec;
		spec.it_interval.tv_sec = 0;
		spec.it_interval.tv_nsec = 0;
		spec.it_value.tv_sec = 0;
		spec.it_value.tv_nsec = 0;

		SetTime( spec );
		}

	//-----------------------------------------------------------------------------------------//
	uint64_t TimerFD::Read()
		{
		uint64_t timerCount = 0;

		Utility::Read( fd_, &timerCount, sizeof( timerCount ) );

		return timerCount;
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::Close()
		{
		if( fd_ != -1 )
			{
			Utility::Close( fd_ );
			fd_ = -1;
			}
		}

	//-----------------------------------------------------------------------------------------//
	int TimerFD::GetFD() const
		{
		return fd_;
		}

	//-----------------------------------------------------------------------------------------//
	void TimerFD::SetTime( struct itimerspec &spec )
		{
		if( timerfd_settime( fd_, TFD_TIMER_ABSTIME, &spec, NULL ) == -1 )
			{
			throw Exception( "timerfd_settime() failed", errno );
			}
		}
	}
