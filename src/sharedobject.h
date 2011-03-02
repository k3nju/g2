#pragma once

#include <dlfcn.h>
#include <string>
#include "uncopyable.h"

namespace g2
	{
	using namespace std;
	
	class SharedObject :private g2::Uncopyable< SharedObject >
		{
		public:
			SharedObject();
			explicit SharedObject( const char *path );
			~SharedObject();

			void Load( int flag = RTLD_NOW );
			void Unload();
			
			void SetPath( const char *path_ );
			void* GetFunction( const char *symbol );
			bool IsLoaded() const;

		private:
			string path_;
			void *handle_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SharedObjectTest

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES=\
$(here)/sharedobject.h\
$(here)/sharedobject.cpp
guts_LDFLAGS=-ldl
</Makefile.am>

<guts>
libs.append( "dl" );
</guts>
 */

#include "exception.h"

using namespace g2;

int GutsEntrySharedObjectTest( int, char** )
	{
	try
		{
		SharedObject so( "/usr/local/lib/libconfig.soxxx" );
		so.Load( RTLD_LAZY );
		//so.GetFunction( "config_write" );
		}
	catch( Exception &E )
		{
		puts( E.GetMsg().c_str() );
		}
	
	return 0;
	}

#endif
#endif


