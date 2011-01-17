#ifndef GENSIN_PatternExceptionH
#define GENSIN_PatternExceptionH

#include "exception.h"

namespace g2
	{
	namespace pattern
		{
		class PatternException :public g2::Exception
			{
			public:
				explicit PatternException( string message = "", int number = -1, int line = -1 );
				PatternException( const PatternException& );
				PatternException& operator = ( const PatternException& );
				virtual ~PatternException();

				int GetLine() const;

			private:
				int line_;
			};
		}
	}

#endif
