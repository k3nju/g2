#include "patternexception.h"

namespace g2
	{
	namespace pattern
		{
		//-----------------------------------------------------------------------------------------//
		PatternException::PatternException( string message, int number, int line )
			:Exception( message, number ), line_( line )
			{
			}

		//-----------------------------------------------------------------------------------------//
		PatternException::PatternException( const PatternException &rhs )
			:Exception( rhs )
			{
			line_ = rhs.line_;
			}

		//-----------------------------------------------------------------------------------------//
		PatternException& PatternException::operator = ( const PatternException &rhs )
			{
			if( this != &rhs )
				{
				Exception::operator = ( rhs );
				line_ = rhs.line_;
				}

			return *this;
			}

		//-----------------------------------------------------------------------------------------//
		PatternException::~PatternException()
			{
			}

		//-----------------------------------------------------------------------------------------//
		int PatternException::GetLine() const
			{
			return line_;
			}
		}
	}
