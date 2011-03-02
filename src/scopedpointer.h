#pragma once

#include "uncopyable.h"

namespace g2
	{
	template < class T >
	class ScopedPointer :private g2::Uncopyable< ScopedPointer< T > >
		{
		public:
			explicit ScopedPointer( T * const pointer );
			~ScopedPointer();

			T* operator -> () const { return pointer_; }
			T& operator *() const { return *pointer_; }
			T* GetPointer() const { return pointer_; }

			void Release();

		private:
			T * const pointer_;
			bool has_ownerShip_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ScopedPointer< T >::ScopedPointer( T * const pointer )
		:pointer_( pointer ), has_ownerShip_( true )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	ScopedPointer< T >::~ScopedPointer()
		{
		if( has_ownerShip_ == true )
			{
			delete pointer_;
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void ScopedPointer< T >::Release()
		{
		has_ownerShip_ = false;
		}
	}

