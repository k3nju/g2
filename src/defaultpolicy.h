#pragma once

#include <cassert>
#include "exception.h"
#include "criticalscope.h"

namespace g2
	{
	namespace default_policy
		{
		//-----------------------------------------------------------------------------------------//
		// Creation Policy
		template < class T > struct CreationPolicy
			{
			static T* Create()
				{
				return new T;
				}
			};

		//-----------------------------------------------------------------------------------------//
		// Destruction Policy
		template < class T > struct DestructionPolicy
			{
			static void Destroy( T *object )
				{
				delete object;
				}
			};
		
		//-----------------------------------------------------------------------------------------//
		// SetterGetter
		template < class T > class PropertyHolder
			{
			protected:
				void Set( T value )
					{
					value_ = value;
					}

				T Get()
					{
					return value_;
					}

				void* GetAddr()
					{
					return static_cast< void* >( &value_ );
					}

			private:
				T value_;
			};
		}
	}







































