#pragma once
#define NetUtility NetUtil

#include <string>
#include <sys/types.h>

#include <netdb.h>
#include "mutexlock.h"
#include "uncopyable.h"

namespace g2
	{
	using namespace std;

	class NetUtility :private g2::Uncopyable< NetUtility >
		{
		public:
			static void SetSockAddrIn( struct sockaddr_in *skadIn, const char *ip, u_short port );

			static struct sockaddr_in* GetSockName( struct sockaddr_in *skadIn, int sock );
			static struct sockaddr_in* GetPeerName( struct sockaddr_in *skadIn, int sock );

			static string GetIP( const struct sockaddr_in *skadIn );
			static u_short GetPort( const struct sockaddr_in *skadIn );

			static int CreateSocket( int domain, int type, int protocol );
			static void SetSockOpt( int sock, int level, int optname, const void *value, socklen_t len );
			static void GetSockOpt( int sock, int level, int optname, void *value, socklen_t *len );
			static int EnableNonBlocking( int sock, bool flag );
			static void EnableReuseAddr( int sock, bool flag );
				
		private:
			NetUtility();
			~NetUtility();

			static g2::MutexLock mLock_;
		};
	}

