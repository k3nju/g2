#include <stdio.h>
#include <cassert>
#include "threading.h"
#include "threadsafequeue.h"
#include "tcpsocket.h"
#include "eventtype.h"
#include "epoll.h"

using namespace g2;
typedef ThreadSafeQueue< int > queue_t;
int threadCount = 10;

const char res[] = "HTTP/1.1 200 OK\r\n"
	"Cache-Control: no-cache\r\n"
	"Content-Length: 1020\r\n"
	"Content-Type: text/html\r\n"
	"Last-Modified: Mon, 16 Mar 2009 20:35:26 GMT\r\n"
	"Accept-Ranges: bytes\r\n"
	"Server: Microsoft-IIS/7.5\r\n"
	"VTag: 27924342100000000\r\n"
	"X-Powered-By: ASP.NET\r\n"
	"Date: Sun, 14 Nov 2010 13:46:21 GMT\r\n"
	"Connection: keep-alive\r\n\r\n";

const size_t resSize = sizeof( res );

struct Handler
	{
		TCPSocket sock;
		
		inline int GetSocket()
			{
			return sock.GetSocket();
			}

		virtual void Handle( int events, Epoll *e ) = 0;
		virtual ~Handler()
			{
			puts( "dtor" );
			}
	};

struct Client : Handler
	{
		size_t sent;
		
		void Init( int fd )
			{
			sock.SetSocket( fd );
			sock.EnableNonBlocking( true );
			sent = 0;
			}

		void Handle( int events, Epoll *e )
			{
			if( events & event_type::ONE_SHOT_READ )
				{
				char buf[1024];
				while( sock.Receive( buf, sizeof( buf ) ) != -1 );
				epoll_data_t d = { this };
				events |= event_type::ONE_SHOT_WRITE;
				}
			
			if( events & event_type::ONE_SHOT_WRITE )
				{
				while( sent < sizeof( res ) )
					{
					ssize_t s = sock.Send( res + sent, sizeof( res ) - sent );
					if( s == -1 )
						{
						break;
						}

					sent += s;
					}

				if( sent < sizeof( res ) )
					{
					epoll_data_t d = { this };
					e->Update( sock.GetSocket(), event_type::ONE_SHOT_WRITE, &d );
					}
				else
					{
					e->Unregister( sock.GetSocket() );
					sock.Close();
					delete this;
					}
				}
			}
	};

struct Acceptor : Handler
	{
	void Init()
		{
		sock.EnableReuseAddr( true );
		sock.Bind( "0.0.0.0", 56000 );
		sock.Listen( 1 );
		sock.EnableNonBlocking( true );
		}

	virtual void Handle( int events, Epoll *e )
		{
		while( events & event_type::ONE_SHOT_ACCEPT )
			{
			int csock = sock.Accept();
			if( csock == -1 )
				{
				epoll_data_t d = { this };
				e->Update( sock.GetSocket(), event_type::ONE_SHOT_ACCEPT, &d );
				break;
				}

			Client *c = new Client();
			c->Init( csock );
			epoll_data_t d = { c };
			fdDist->Register( csock, event_type::ONE_SHOT_READ, &d );
			}
		}
	};

class T :public Threading
	{
	public:
		Epoll e;
		
		virtual int Thread( void  *argv )
			{
			try
				{
				while( 1 )
					{
					epoll_event events[1024];
					int actives = e.Select( events, 1024, -1 );
					printf( "%x %d\n", (unsigned int)GetThreadID(), actives );
					if( actives <= 0 )
						{
						continue;
						}
					
					for( int i = 0; i < actives; ++i )
						{
						epoll_event *eve = events + i;
						Handler *h = (Handler*)( eve->data.ptr );
						h->Handle( eve->events, &e );
						}
					}
				}
			catch( Exception &E )
				{
				E.Output();
				}
			
			return 0;
			}
	};

struct FDDistribution
	{
		T *threads;
		
		void SetThreads( T *t )
			{
			threads = t;
			}
		
		void Register( int fd, int event, epoll_data_t *data )
			{
			static unsigned int n = 0;
			int m = __sync_fetch_and_add( &n, 1 );
			threads[ m % threadCount ].e.Register( fd, event, data );
			}
		
	};


int main()
	{
	try
		{
		T threads[threadCount];
		FDDistribution fdd;
		fdDist = &fdd;
		fdDist->SetThreads( threads );
		
		Acceptor a;
		a.Init();
		
		for( int i = 0; i < threadCount; ++i )
			{
			threads[i].Create();
			}
		
		epoll_data_t data;
		data.ptr = &a;
		fdDist->Register( a.sock.GetSocket(), event_type::ONE_SHOT_ACCEPT, &data );
		
		for( int i = 0; i < threadCount; ++i )
			{
			threads[i].Join();
			}
		}
	catch( Exception &E )
		{
		E.Output();
		}
		
	return 0;
	}
