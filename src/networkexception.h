#pragma once

#include "exception.h"
#include <string>

namespace g2
	{
	enum failure_code
		{
		UNKNOWN_FAILURE = 1000,
		SOCKET_FAILURE,
		BIND_FAILURE,
		LISTEN_FAILURE,
		ACCEPT_FAILURE,
		CONNECT_FAILURE,
		WRITE_FAILURE,
		READ_FAILURE,
		SEND_FAILURE,
		RECEIVE_FAILURE,
		CLOSE_FAILURE,
		POLL_FAILURE,
		NAME_RESOLUTION_FAILURE,
		GET_SOCKOPT_FAILURE,
		SET_SOCKOPT_FAILURE,
		GET_SOCKNAME_FAILURE,
		GET_PEERNAME_FAILURE,
		GET_ACTIVE_SOCKETS_FAILURE,
		SHUTDOWN_FAILURE
		};
		
	typedef failure_code FAILURE_CODE;
		
	class NetworkException :public g2::Exception
		{
		public:
			explicit NetworkException( string message = "", int number = -1, int code = -1 );
			NetworkException( const NetworkException& );
			NetworkException& operator = ( const NetworkException& );
			virtual ~NetworkException() throw();
		};
	}

