#pragma once

#include <utility>
#include <vector>
#include <boost/function.hpp>
#include "eventtype.h"
#include "mutexlock.h"
#include "criticalscope.h"
#include "epoll.h"
#include "epollevent.h"
#include "memorypool.h"
#include "threadsafeflag.h"
#include "eventfd.h"
#include "uncopyable.h"
#include "sockethashmap.h"

namespace g2
	{
	class IODispatch
		{
		public:
			typedef boost::function< bool () > handler_t;
				
			IODispatch();
			~IODispatch();

			void Request( int socket, g2::event_type::TYPE type, handler_t &handler );
			void Request( int socket, g2::event_type::TYPE type );
			void Cancel( int socket );
			void Cancel( int socket, g2::event_type::TYPE type );
			void SetErrorHandler( int socket, handler_t &handler );

			void Dispatch( int timeout );
			void Stop();
			
		private:
			struct EntryItem
				{
					EntryItem();
					~EntryItem(){};
						
					int events;
					handler_t handler;

					static bool DefaultHandler();
				};

			void SetHandler( struct EntryItem &entry, int socket, handler_t &handler );

			typedef g2::SocketHashMap< struct EntryItem > entries_t;
			entries_t inputEntries_;
			g2::MutexLock inputEntryLock_;			
			entries_t outputEntries_;
			g2::MutexLock outputEntryLock_;
			entries_t errorEntries_;
			g2::MutexLock errorEntryLock_;
			
			g2::Epoll inputEpoll_;
			g2::Epoll outputEpoll_;
			bool OutputEpollHandler();

			g2::ThreadSafeFlag stopFlag_;
			g2::EventFD stopEvent_;
			bool StopHandler();

			typedef g2::CriticalScope< g2::MutexLock > critical_scope_t;

			void SetMembersByType( g2::event_type::TYPE type,
								   entries_t *&entries,
								   g2::Epoll *&epoll,
								   critical_scope_t &cs );
			bool Invoke( int socket,
						 g2::MutexLock &cs,
						 entries_t &entries );
						 
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__IODispatchTest

#include <cstdio>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include "tcpsocket.h"


/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/iodispatch.h \
$(here)/iodispatch.cpp
</Makefile.am>
*/

using namespace g2;
using namespace boost;
using namespace std;

typedef IODispatch::handler_t handler_t;
TCPSocket Sock;

bool Func()
	{
	const char *buf = "HEAD / HTTP/1.0\r\n\r\n";
	Sock.Send( buf, strlen( buf ) );
	
	return false;
	}
bool Func2()
	{
	char buf[100] = "";
	Sock.Receive( buf, sizeof( buf ) -1 );
	puts( buf );
	Sock.Close();

	return false;
	}

handler_t handler = handler_t( Func );

class Test :public boost::enable_shared_from_this< Test >
	{
	public:
		bool Handle()
			{
			return true;
			}

		template < class T >
		void SetDispatcher( T &t )
			{
			handler_t h = boost::bind( &Test::Handle, this->shared_from_this() );
			t.Request( 1, event_type::ONE_SHOT_WRITE, h );
			}

		template < class T >
		void Cancel( T &t )
			{
			t.Cancel( 1 );
			}
	};

#include <sstream>

int GutsEntryIODispatchTest( int, char** )
	{
	shared_ptr<Test> t( new Test );
	{
	IODispatch d;
	t->SetDispatcher( d );
	}
	
	assert( t.use_count() == 1 );
	t.reset();
	assert( t.use_count() == 0 );
	
	
	/*
	try
		{
		Sock.Connect( "www.microsoft.com", 80 );
		IODispatch D;
		D.Request( Sock.GetSocket(), event_type::ONE_SHOT_WRITE, handler );
		D.Dispatch( -1 );
		handler = handler_t ( Func2 );
		D.Request( Sock.GetSocket(), event_type::ONE_SHOT_READ,  handler );
		D.Dispatch( -1 );		
		}
	catch( Exception &E )
		{
		E.Output();
		throw;
		}
	*/
	
	puts( "End" );

	return 0;
	}

#endif
#endif
