#pragma once

#include <sys/types.h>
#include <map>
#include <string>
#include "uncopyable.h"

namespace g2
	{
	class INotify
		{
		private:
			G2_MARK_UNCOPYABLE( INotify );
			
		public:
			INotify();
			~INotify();

			void Add( const char *path, uint32_t mask );
			void Remove( const char *path );
			void Close();
			ssize_t Read( char *buf, size_t size );

			int GetFD() const;
			void EnableNonBlocking( bool flag );

		private:
			int fd_;
			typedef std::map< std::string, int > watch_map_t;
			watch_map_t watchMap_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__INotifyTest

#include "iodispatch.h"
#include <sys/inotify.h>
#include <boost/bind.hpp>

using namespace g2;

struct Handler
	{
		INotify in;
		bool Handle()
			{
			static size_t count = 0;
			
			char buf[1000];
			ssize_t sizeRead = in.Read( buf, sizeof( buf ) );
			ssize_t i = 0;
			while( i < sizeRead )
				{
				inotify_event *eve = (inotify_event*)buf;
				printf( "wd     = %d\n"
						"mask   = %x\n"
						"cookie = %x\n"
						"len    = %x\n"
						"name   = %s\n",
						eve->wd, eve->mask, eve->cookie, eve->len, eve->name );
				i += sizeof( *eve ) + eve->len;
				}

			if( count++ > 1 )
				{
				in.Remove( "/tmp" );
				puts( "removed" );
				//return false;
				}
			
			return true;
			}
	};

int GutsEntryINotifyTest(int,char**)
	{

	Handler h;
	h.in.Add( "/tmp", IN_CREATE );
	IODispatch d;
	IODispatch::handler_t handler = boost::bind( &Handler::Handle, &h ); 
	d.Request( h.in.GetFD(),event_type::READ, handler );
	d.Dispatch( -1 );
	
	return 0;
	}

#endif
#endif
