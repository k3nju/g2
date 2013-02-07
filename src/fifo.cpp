#include "fifo.h"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "exception.h"

#include <stdio.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Fifo::Fifo():FDBase()
		{
		}

	//-----------------------------------------------------------------------------------------//
	Fifo::Fifo( const char *pathname, mode_t fifoMode, int openFlags )
		:FDBase()
		{
		Open( pathname, fifoMode, openFlags );
		}

	//-----------------------------------------------------------------------------------------//
	Fifo::~Fifo()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Fifo::Open( const char *pathname, mode_t fifoMode, int openFlags )
		{
		struct stat unused;
		if( stat( pathname, &unused ) == -1 )
			{
			if( mkfifo( pathname, fifoMode ) == -1 )
				{
				throw Exception( "mkfifo() failed", errno );
				}
			}
		
		int fd = 0;
		
		do
			{
			fd = open( pathname, openFlags );
            if( fd == -1 && errno != EINTR )
				{
				throw Exception( "open() failed", errno );
				}
			}
		while( fd == -1 && errno == EINTR );
		
		ResetFD( fd );
		}
	}
