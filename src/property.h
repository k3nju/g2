#pragma once

#include "defaultpolicy.h"

namespace g2
	{
	template < class T, template<class> class PropertyHolder = g2::default_policy::PropertyHolder >
	class Property :private PropertyHolder< T >
		{
		public:
			Property();
			explicit Property( T value );
			Property( const Property& );
			Property& operator = ( const Property& );
			Property& operator = ( const T& );
			~Property();

			operator T();
			T GetValue();
		};

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >::Property():PropertyHolder< T >()
		{}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >::Property( T value ):PropertyHolder< T >( value )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >::Property( const Property &rhs ):PropertyHolder< T >( rhs )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >&
	Property< T, PropertyHolder >::operator =( const Property< T, PropertyHolder > &rhs )
		{
		if( this != &rhs )
			{
			PropertyHolder< T >::operator = ( rhs );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >&
	Property< T, PropertyHolder >::operator = ( const T &value )
		{
		if( PropertyHolder< T >::GetAddr() != &value )
			{
			PropertyHolder< T >::operator = ( value );
			}

		return *this;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >::~Property()
		{}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > Property< T, PropertyHolder >::operator T()
		{
		return PropertyHolder< T >::Get();
		}

	//-----------------------------------------------------------------------------------------//
	template < class T, template<class> class PropertyHolder > T Property< T, PropertyHolder >::GetValue()
		{
		return PropertyHolder< T >::Get();
		}
	}

