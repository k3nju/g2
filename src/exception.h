#pragma once

#include <string>
#include <sstream>
#include "debugutility.h"

namespace g2
	{
	using namespace std;

	class Exception
		{
		public:
			// number should be errno
			// code can be set various value that you like
			explicit Exception( string message = "", int number = -1, int code = -1 );
			Exception( const Exception& );
			Exception& operator = ( const Exception& );
			virtual ~Exception();

			// set location informations
			void SetLocInfo( const std::string &file, int line, const std::string &function );

			string GetMsg() const;
			string GetMessage() const;
			int GetNumber() const;
			int GetCode() const;

			std::string GetBundledMessage() const;
			void Output() const;
			void Output( const char* const fileName, unsigned int line, const char* const funcName );
			
		private:
			string message_;
			int number_;
			int code_;

			string file_;
			int line_;
			string function_;

			DebugUtil::BackTrace backTrace_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ExceptionTest

using namespace g2;

int GutsEntryExceptionTest(int,char**)
	{
	Exception e;
	e.Output();
	puts( "--2--" );
	e.Output( "testfile", 100, "funcname" );

	return 0;
	}

#endif
#endif

