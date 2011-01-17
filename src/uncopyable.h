#pragma once

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	class Uncopyable
		{
		public:
			Uncopyable();
			~Uncopyable();
			
		private:
			Uncopyable( const Uncopyable & );
			Uncopyable& operator = ( const Uncopyable & );
		};
		//-----------------------------------------------------------------------------------------//
	// Another method.
#define G2_MARK_UNCOPYABLE( className )			\
	className( const className& );				\
	className& operator = ( const className& )
	}
