#pragma once

#include <sys/uio.h>
#include <string.h>
#include <sys/time.h>
#include "uncopyable.h"
#include "netutility.h"
#include "networkexception.h"
#include "addrinfo.h"
#include "utility.h"

namespace g2
	{
	template < class StreamImpl >
	class SocketBase :private g2::Uncopyable
		{
		public:
			typedef SocketBase< StreamImpl > self_t;
			
			void CreateSocket( int type );
			void CreateSocket();
				
			void Bind( const char *addr, unsigned short port );
			void Close();

			ssize_t Send( const char *buf );
			ssize_t Send( const char *buf, size_t size );
			ssize_t Send( const struct iovec *iov, size_t count );
			ssize_t Receive( char *buf, size_t size );
			ssize_t Receive( struct iovec *iov, size_t count );
			
			inline int GetSocket() const;
			inline void SetSocket( int socket );
			inline void MarkInvalid();
				
			void EnableNonBlocking( bool flag );
			void EnableReuseAddr( bool flag );
			void GetTimeout( const struct timeval &timeout );			
			void SetTimeout( const struct timeval &timeout );
			void GetOption( int level, int optname, void *optval, socklen_t *optlen );
			void SetOption( int level, int optname, const void *optval, socklen_t optlen );

		protected:
			SocketBase();
			explicit SocketBase( int socket );
				
		private:
			int socket_;
		};
		
	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	SocketBase< StreamImpl >::SocketBase()
		:Uncopyable(),
		 socket_( -1 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	SocketBase< StreamImpl >::SocketBase( int socket )
		:Uncopyable(),
		 socket_( socket )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::CreateSocket( int type )
		{
		if( socket_ == -1 )
			{
			socket_ = NetUtil::CreateSocket( AF_INET, type, 0 );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::CreateSocket()
		{
		if( socket_ == -1 )
			{
			socket_ = NetUtil::CreateSocket( AF_INET, StreamImpl::SOCKET_TYPE, 0 );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::Bind( const char *addr, unsigned short port )
		{
		string portStr = Utility::Convert2String( port );
		AddrInfo ni( AF_INET, StreamImpl::SOCKET_TYPE, AddrInfo::SERVER, addr, portStr.c_str() );
		const struct addrinfo *res = ni.Get();
		
		if( bind( socket_, res->ai_addr, res->ai_addrlen ) == -1 )
			{
			throw NetworkException( "bind() failed", errno, BIND_FAILURE );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::Close()
		{
		if( socket_ == -1 )
			{
			return;
			}

		int result = 0;
		do
			{
			result = close( socket_ );
			if( result == -1 && errno != EINTR )
				{
				throw NetworkException( "close() failed", errno, CLOSE_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );
			
		socket_ = -1;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	ssize_t SocketBase< StreamImpl >::Send( const char *buf )
		{
		return Send( buf, strlen( buf ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	ssize_t SocketBase< StreamImpl >::Send( const char *buf, size_t size )
		{
		ssize_t result = 0;
		
		do
			{
			result = static_cast< StreamImpl* >( this )->SendImpl( buf, size );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "Send( const char*, size_t ) failed", errno, SEND_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );
			
		return result;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	ssize_t SocketBase< StreamImpl >::Send( const struct iovec *iov, size_t count )
		{
		ssize_t result = 0;
		
		do
			{
			result = static_cast< StreamImpl* >( this )->SendImpl( iov, count );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "Send( struct iovec*, size_t ) failed", errno, SEND_FAILURE );
				}
			}				
		while( result == -1 && errno == EINTR );

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	ssize_t SocketBase< StreamImpl >::Receive( char *buf, size_t size )
		{
		ssize_t result = 0;
		
		do
			{
			result = static_cast< StreamImpl* >( this )->ReceiveImpl( buf, size );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "Receive() failed", errno, RECEIVE_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	ssize_t SocketBase< StreamImpl >::Receive( struct iovec *iov, size_t count )
		{
		ssize_t result = 0;

		do
			{
			result = static_cast< StreamImpl* >( this )->ReceiveImpl( iov, count );
			if( result == -1 && errno != EAGAIN && errno != EINTR )
				{
				throw NetworkException( "Receive() failed", errno, RECEIVE_FAILURE );
				}
			}
		while( result == -1 && errno == EINTR );

		return result;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	int SocketBase< StreamImpl >::GetSocket() const
		{
		return socket_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::SetSocket( int socket )
		{
		if( socket_ != -1 )
			{
			Close();
			}
		
		socket_ = socket;
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::MarkInvalid()
		{
		socket_ = -1;
		}		

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::EnableNonBlocking( bool flag )
		{
		NetUtil::EnableNonBlocking( socket_, flag );
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::EnableReuseAddr( bool flag )
		{
		NetUtil::EnableReuseAddr( socket_, flag );
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::GetTimeout( const struct timeval &timeout )
		{
		GetOption( SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout ) );
		}	

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::SetTimeout( const struct timeval &timeout )
		{
		SetOption( SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof( timeout ) );
		SetOption( SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof( timeout ) );		
		}
	
	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::GetOption( int level, int optname, void *optval, socklen_t* optlen )
		{
		NetUtil::GetSockOpt( socket_, level, optname, optval, optlen );
		}

	//-----------------------------------------------------------------------------------------//
	template < class StreamImpl >
	void SocketBase< StreamImpl >::SetOption( int level, int optname, const void *optval, socklen_t optlen )
		{
		NetUtil::SetSockOpt( socket_, level, optname, optval, optlen );
		}
	}

