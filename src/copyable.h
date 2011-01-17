#pragma once

#include <stdio.h>
#include <cassert>
#include "referencemanagement.h"

namespace g2
	{
	template < class T >
	class Copyable :public g2::ReferenceManagement< Copyable< T > >
		{
			typedef g2::ReferenceManagement< Copyable< T > > base_t;
		public:
			Copyable();
			template < class T0 >
			Copyable( const T0 &t0 );
			template < class T0, class T1 >
			Copyable( const T0 &t0, const T1 &t1 );
			template < class T0, class T1, class T2 >
			Copyable( const T0 &t0, const T1 &t1, const T2 &t2 );
			template < class T0, class T1, class T2, class T3 >
			Copyable( const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3 );
			template < class T0, class T1, class T2, class T3, class T4 >
			Copyable( const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4 );
			Copyable( const Copyable &rhs );
			Copyable& operator = ( const Copyable &rhs );
			~Copyable();

			void Delegate( const Copyable &rhs );
			void Dispose();

			operator const T& () { return *object_; }
			operator T& (){ return *object_; }
			const T* operator ->() const { return object_; }
			T* operator ->() { return object_; }
			const T& operator *()  const { return *object_; }
			T& operator *() { return *object_; }
			const T& Get() const { return *object_; }
			T& Get() { return *object_; }

		private:
			T *object_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	Copyable< T >::Copyable():base_t(), object_( NULL )
		{
		object_ = new T();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class T0 >
	Copyable< T >::Copyable( const T0 &t0 ):base_t(), object_( NULL )
		{
		object_ = new T( t0 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class T0, class T1 >
	Copyable< T >::Copyable( const T0 &t0, const T1 &t1 ):base_t(), object_( NULL )
		{
		object_ = new T( t0, t1 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class T0, class T1, class T2 >
	Copyable< T >::Copyable( const T0 &t0, const T1 &t1, const T2 &t2 ):base_t(), object_( NULL )
		{
		object_ = new T( t0, t1, t2 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class T0, class T1, class T2, class T3 >
	Copyable< T >::Copyable( const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3 ):base_t(), object_( NULL )
		{
		object_ = new T( t0, t1, t2, t3 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class T0, class T1, class T2, class T3, class T4 >
	Copyable< T >::Copyable( const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3, const T4 &t4 ):base_t(), object_( NULL )
		{
		object_ = new T( t0, t1, t2, t3, t4 );
		}


	//-----------------------------------------------------------------------------------------//
	template < class T >
	Copyable< T >::Copyable( const Copyable &rhs ):base_t( rhs ) //, object_( NULL ) # don't comment in me
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	Copyable< T >& Copyable< T >::operator = ( const Copyable< T > &rhs )
		{
		if( this != &rhs )
			{
			base_t::operator = ( rhs );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	Copyable< T >::~Copyable()
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void Copyable< T >::Delegate( const Copyable< T > &rhs )
		{
		// just write copy code
		assert( rhs.object_ != NULL );
		object_ = rhs.object_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void Copyable< T >::Dispose()
		{
		// just write destruction code
		assert( object_ != NULL );
		delete object_;
		object_ = NULL;
		}
	}


	
#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__CopyableTest

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/copyable.h \
$(here)/copyable.cpp
</Makefile.am>
*/

#include "uncopyable.h"
using namespace g2;

class U :private Uncopyable
	{
	public:
		U()
			{
			puts( "ctor" );
			}
		
		~U()
			{
			puts( "dtor" );
			}
	};

typedef Copyable< U > val_t;

val_t F( val_t v )
	{
	val_t tmp = v;
	return v;
	}
	

int GutsEntryCopyableTest(int,char**)
	{
	val_t v;
	val_t v2 = v;
	v = F( v );
	
	return 0;
	}


#endif
#endif
