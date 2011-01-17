#pragma once

#include <stdio.h>

namespace g2
	{
	template < class T >
	class ReferenceManagement
		{
		protected:
			ReferenceManagement();
			ReferenceManagement( const ReferenceManagement &rhs );
			ReferenceManagement& operator = ( const ReferenceManagement &rhs );
			~ReferenceManagement();
			
			unsigned int GetCount() const;

			// sub classes are required two member function.
			// 1st. "void Delegate( const T &rhs )"
			// Delegate is called when copy happend.
			// 2nd. "void Dispose()"
			// Dispose is called when object is be destructed
			// state for calling these are controlled by this class.
			// so, sub class implement only "copy process code" for Delegate,
			// "destruct process code" for Dispose.
			
		private:
			void AddCount();
			void RemoveCount();

			unsigned int *count_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ReferenceManagement< T >::ReferenceManagement()
		:count_( NULL )
		{
		count_ = new unsigned int;
		*count_ = 0;
		AddCount();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ReferenceManagement< T >::ReferenceManagement( const ReferenceManagement< T > &rhs )
		{
		count_ = rhs.count_;
		AddCount();
		static_cast< T* >( this )->Delegate(
			static_cast< const T& >( rhs )
			);
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ReferenceManagement< T >& ReferenceManagement< T >::operator = ( const ReferenceManagement &rhs )
		{
		if( this != &rhs )
			{
			RemoveCount();
			if( *count_ == 0 )
				{
				static_cast< T* >( this )->Dispose();
				
				delete count_;
				}

			count_ = rhs.count_;
			AddCount();
			static_cast< T* >( this )->Delegate(
				static_cast< const T& >( rhs )
				);
			}
		
		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ReferenceManagement< T >::~ReferenceManagement()
		{
		RemoveCount();
		if( GetCount() == 0 )
			{
			static_cast< T* >( this )->Dispose();
			delete count_;
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ReferenceManagement< T >::AddCount()
		{
		++(*count_);
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ReferenceManagement< T >::RemoveCount()
		{
		--(*count_);
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	unsigned int ReferenceManagement< T >::GetCount() const
		{
		return *count_;
		}	
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__RefCountTest

#include <stdio.h>
#include <deque>
#include <cassert>
using namespace std;

/*
<Makefile.am>
here=/home/kizi/devel/gensinx/g2/src
guts_SOURCES=\
$(here)/referencemanagement.h\
$(here)/referencemanagement.cpp
</Makefile.am>
*/

// resource

class T :public g2::ReferenceManagement< T >
	{
		typedef g2::ReferenceManagement< T > base_t;
		int *p_;
		
	public:
		T():base_t(),p_( NULL )
			{
			p_ = new int;
			}
		
		T( const T &rhs ):base_t( rhs )
			{
			}

		T& operator = ( const T &rhs )
			{
			if( this != &rhs )
				{
				base_t::operator = ( rhs );
				}

			return *this;
			}

		void Dispose()
			{
			// just delete
			printf( "delete %x\n", p_ );
			delete p_;
			}

		void Delegate( const T &rhs )
			{
			// just copy
			printf( "delegate %x\n", rhs.p_ );
			p_ = rhs.p_;
			}
	};

void F( deque<T> l )
	{
	T a = l.front();
	T b = a;
	for( int i = 0; i < 10; ++i )
		{
		l.pop_front();
		}
	T c = a;
	}

T Test( T a )
	{
	return a;
	}

int GutsEntryRefCountTest(int,char**)
	{
	T t;
	t = Test( t );
	
	return 0;
	}

#endif
#endif
