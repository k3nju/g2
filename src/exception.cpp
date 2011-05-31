#include "exception.h"
#include <error.h>

namespace g2
	{
    //////////////////////////////////////////////////
	//	Exception(string message_ = "", int code = -1, int number = 0 )
	//
	Exception::Exception( string message, int number, int code )
		:runtime_error( message ),
		 message_( message ), number_( number ), code_( code ),
		 file_(), line_( -1 ), function_(), backTrace_()
		{
		}

	//////////////////////////////////////////////////
	//	Exception(Exception &)
	//
	Exception::Exception( const Exception &rhs )
		:runtime_error( rhs.message ),
		 message_( rhs.message_ ), number_( rhs.number_ ), code_( rhs.code_ ),
		 file_( rhs.file_ ), line_( rhs.line_ ), function_( rhs.function_ ),
		 backTrace_( rhs.backTrace_ )
		{
		}

	//-----------------------------------------------------------------------------------------//
	Exception& Exception::operator = ( const Exception &rhs )
		{
		if( this != &rhs )
			{
			runtime_error::operator ( rhs );
			message_	= rhs.message_;
			number_     = rhs.number_;
			code_		= rhs.code_;
			file_       = rhs.file_;
			line_       = rhs.line_;
			function_   = rhs.function_;
			backTrace_  = rhs.backTrace_;
			}
		
		return *this;
		}

	//////////////////////////////////////////////////
	//	~Exception()
	//
	Exception::~Exception()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void Exception::SetLocInfo( const std::string &file, int line, const std::string &function )
		{
		file_ = file;
		line_ = line;
		function_ = function;
		}

	//////////////////////////////////////////////////
	//	string GetMsg()
	//
	string Exception::GetMsg() const { return message_; }

	//////////////////////////////////////////////////
	// string GetMessage() const
	// 
	string Exception::GetMessage() const { return message_; }	

	/////////////////////////////////////////////////////
	//  int GetNumber()
	//
	int Exception::GetNumber() const { return number_; }

	//////////////////////////////////////////////////////
	//	int GetCode()
	//
	int Exception::GetCode() const { return code_; }

	/////////////////////////////////////////////////////
	// wow. good 01d comment. :-) on 2009/06/05
	//
	std::string Exception::GetBundledMessage() const
		{
		std::stringstream ss;

		ss << "\n";
		ss << "[!] Exception general message\n";
		ss << "    Message  :" << message_ << "\n";
		ss << "    Number   :" << number_ << "\n";
		ss << "    Code     :" << code_ << "\n";
		ss << "    File     :" << file_ << "\n";
		ss << "    Line     :" << line_ << "\n";
		ss << "    Function :" << function_ << "\n";
		ss << "[!] Back trace\n";

		std::string tmp = backTrace_.GetOutputString();
		size_t pos = 0;
		while( ( pos = tmp.find( "\n" ) ) != std::string::npos )
			{
			ss << "    " << tmp.substr( 0, pos + 1 );
			tmp = tmp.substr( pos + 1 );
			}

		ss << "    " << tmp;

		return ss.str();
		}

	//-----------------------------------------------------------------------------------------//
	void Exception::Output() const
		{
		error( 0,
			   number_ == -1 ? 0 : number_,
			   "%s",
			   GetBundledMessage().c_str() );
		}

	//-----------------------------------------------------------------------------------------//
	void Exception::Output( const char* const fileName, unsigned int line, const char* const funcName )
		{
		std::stringstream ss;
		ss << GetBundledMessage() << "\n";
		ss << "[!] Caught location message\n";
		ss << "    FileName :" << fileName << "\n";
		ss << "    Line     :" << line << "\n";
		ss << "    Function :" << funcName << "\n";
		ss << "    Error message";
		
		error( 0,
			   number_ == -1 ? 0 : number_,
			   "%s",
			   ss.str().c_str() );
		}
	}

