#include "daemon.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	Daemon::Daemon( const char *chdir )//:Uncopyable(),
		:chdir_ ( chdir )
		{
		assert( chdir != NULL && "chdir is NULL" );
		}

	//-----------------------------------------------------------------------------------------//
	Daemon::~Daemon()
		{
		}

	//-----------------------------------------------------------------------------------------//
	bool Daemon::BecomeDaemon()
		{
		// prevent being process leader
		switch( fork() )
			{
			case 0: // child process
				break;

			case -1: // error
				return false;
				break;
				
			default: // default. parent process
				exit( 0 );
				break;
			}

		// becom session leader
		if( setsid() == -1 )
			{
			return false;
			}

		// close all descriptors
		for( int i = getdtablesize(); i >= 0; --i )
			{
			close( i );
			}

		// set umask 0
		umask( 0 );

		// change directory
		if( chdir( chdir_ ) == -1 )
			{
			return false;
			}

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	int Daemon::RedirectSTDIN( int in )
		{
		return dup2( in, 0 );
		}

	//-----------------------------------------------------------------------------------------//
	int Daemon::RedirectSTDOUT( int out )
		{
		return dup2( out, 1 );
		}

	//-----------------------------------------------------------------------------------------//
	int Daemon::RedirectSTDERR( int err )
		{
		return dup2( err, 2 );
		}
	
	}

