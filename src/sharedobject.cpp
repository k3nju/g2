#include "sharedobject.h"
#include <errno.h>
#include <sstream>
#include <cassert>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	SharedObject::SharedObject():path_(), handle_( NULL )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	SharedObject::SharedObject( const char *path ):path_( path ), handle_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	SharedObject::~SharedObject()
		{
		if( handle_ != NULL )
			{
			try
				{
				Unload();
				}
			catch( ... )
				{
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	void SharedObject::Load( int flag )
		{
		void *handle = dlopen( path_.c_str(), flag );
		if( handle == NULL )
			{
			string msg( "dlopen() failed. " );
			msg += string( dlerror() );
			
			throw Exception( msg, errno );
			}

		handle_ = handle;
		}

	//-----------------------------------------------------------------------------------------//
	void SharedObject::Unload()
		{
		assert( handle_ != NULL );
		
		if( dlclose( handle_ ) != 0 )
			{
			string msg( "dlclose() failed. " );
			msg += string( dlerror() );
			
			throw Exception( msg, errno );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void SharedObject::SetPath( const char *path )
		{
		path_ = path;
		}

	//-----------------------------------------------------------------------------------------//
	void* SharedObject::GetFunction( const char *symbol )
		{
		// clear previous error status
		dlerror();

		void *tmp = dlsym( handle_, symbol );
		char *error_msg = dlerror();

		if( error_msg != NULL )
			{
			string msg( "dlsym() failed. " );
			msg += string( error_msg );
			
			throw Exception( msg, errno );
			}

		return tmp;
		}

	//-----------------------------------------------------------------------------------------//
	bool SharedObject::IsLoaded() const
		{
		return handle_ != NULL;
		}
	}
