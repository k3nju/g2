#pragma once

#include <boost/shared_ptr.hpp>
#include "uncopyable.h"

namespace g2
	{
	template < class T >
	class SingletonAdaptation :private g2::Uncopyable
		{
		public:
			static void Instantiate();

			template < class A0 >
			static void Instantiate( const A0 &a0 );

			template < class A0, class A1 >
			static void Instantiate( const A0 &a0, const A1 &a1 );
				
			template < class A0, class A1, class A2 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2 );
				
			template < class A0, class A1, class A2, class A3 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3 );
				
			template < class A0, class A1, class A2, class A3, class A4 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 );
				
			template < class A0, class A1, class A2, class A3, class A4, class A5 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5 );
				
			template < class A0, class A1, class A2, class A3, class A4, class A5, class A6 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6 );
				
			template < class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7 );
				
			template < class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
			static void Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8 );

			static T& GetInstance();
			static void Destroy();
				
		private:
			static boost::shared_ptr< T > instance_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	boost::shared_ptr< T > SingletonAdaptation< T >::instance_;
		
	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SingletonAdaptation< T >::Instantiate()
		{
		instance_.reset( new T() );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0 )
		{
		instance_.reset( new T( a0 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1 )
		{
		instance_.reset( new T( a0, a1 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2 )
		{
		instance_.reset( new T( a0, a1, a2 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3 )
		{
		instance_.reset( new T( a0, a1, a2, a3 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3, class A4 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4 )
		{
		instance_.reset( new T( a0, a1, a2, a3, a4 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3, class A4, class A5 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5 )
		{
		instance_.reset( new T( a0, a1, a2, a3, a4, a5 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3, class A4, class A5, class A6 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6 )
		{
		instance_.reset( new T( a0, a1, a2, a3, a4, a5, a6 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7 )
		{
		instance_.reset( new T( a0, a1, a2, a3, a4, a5, a6, a7 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0, class A1, class A2, class A3, class A4, class A5, class A6, class A7, class A8 >
	void SingletonAdaptation< T >::Instantiate( const A0 &a0, const A1 &a1, const A2 &a2, const A3 &a3, const A4 &a4, const A5 &a5, const A6 &a6, const A7 &a7, const A8 &a8 )
		{
		instance_.reset( new T( a0, a1, a2, a3, a4, a5, a6, a7, a8 ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T& SingletonAdaptation< T >::GetInstance()
		{
		return *instance_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SingletonAdaptation< T >::Destroy()
		{
		instance_.reset();
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SingletonAdaptationTest
#include <stdio.h>


using namespace g2;
using namespace g2;

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devs/draft_singletonadaptation.h \
/home/kizi/devs/draft_singletonadaptation.cpp \
/home/kizi/devs/src/smartpointer.h \
/home/kizi/devs/src/smartpointer.cpp
</Makefile.am>
 */

class Test0
	{
	public:
		Test0()
			{
			puts( "Test()" );
			}
		~Test0()
			{
			puts( "~Test()" );
			}

		void Method()
			{
			puts( "Test0" );
			}
	};

class Test1
	{
	public:
		Test1( int )
			{
			puts( "Test( int )" );
			}

		~Test1()
			{
			puts( "~Test1()" );
			}

		void Method()
			{
			puts( "Test1" );
			}
	};

class Test2
	{
	public:
		Test2( int, short, const char* )
			{
			puts( "Test( int, short, char* )" );
			}

		~Test2()
			{
			puts( "~Test2()" );
			}

		void Method()
			{
			puts( "Test2" );
			}
	};

int GutsEntrySingletonAdaptationTest( int, char** )
	{

	{
	// phase 0
	typedef SingletonAdaptation< Test0 > SingAdapt;
	SingAdapt::Instantiate();
	SingAdapt::GetInstance().Method();	
	}

	{
	//phase 1
	typedef SingletonAdaptation< Test1 > SingAdapt;
	SingAdapt::Instantiate( 1 );
	SingAdapt::GetInstance().Method();
	}

	{
	//phase 2
	typedef SingletonAdaptation< Test2 > SingAdapt;
	SingAdapt::Instantiate( (int)1, (short)2, "3" );
	SingAdapt::GetInstance().Method();
	}

	return 0;
	}

	
#endif
#endif
