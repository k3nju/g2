#pragma once

#include <typeinfo>
#include <algorithm>
#include "exception.h"

namespace g2
	{
	using namespace std;
	class AbstractHolder;
	template < class > class Holder;

	//-----------------------------------------------------------------------------------------//
	class Any
		{
		public:
			Any();
			Any( const Any &rhs );
			template < class T > Any( T &value );
			Any& operator = ( const Any &rhs );
			template < class T > Any& operator = ( T &rhs );
			~Any();

			template < class T > T Cast();

		private:
			Any& Copy( const Any &rhs );
			AbstractHolder *holder_;
		};

	//-----------------------------------------------------------------------------------------//
	class AbstractHolder
		{
		public:
			virtual ~AbstractHolder(){};
			virtual const type_info& Type() = 0;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T > class Holder :public AbstractHolder
		{
		public:
			Holder( T &value ):value_( value ){};
			virtual ~Holder(){};

			virtual const type_info& Type(){ return typeid( T ); }

		private:
			T value_;
		};

	//-----------------------------------------------------------------------------------------//
	Any::Any():holder_( NULL )
		{}

	//-----------------------------------------------------------------------------------------//
	Any::Any( const Any &rhs )
		{
		Copy( rhs );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > Any::Any( T &value )
			:holder_( new Holder< T >( value ) )
		{}

	//-----------------------------------------------------------------------------------------//
	Any& Any::operator = ( const Any &rhs )
		{
		Copy( rhs );
		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T > Any& Any::operator = ( T &rhs )
		{
		Copy( Any( rhs ) );
		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	Any::~Any()
		{
		delete holder_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T Any::Cast()
		{
		if( holder_ == NULL )
			{
			return typeid( void );
			}

		if( holder_->Type() != typeid( T ) )
			{
			throw Exception( "Any::Cast< T >() failed" );
			}

		return static_cast< Holder< T >* >( holder_ )->value_;
		}

	//-----------------------------------------------------------------------------------------//
	Any& Any::Copy( const Any &rhs )
		{
		Any tmp( rhs );
		swap( *this, tmp );
		
		return *this;
		}
	}

