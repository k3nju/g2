#pragma once

#include <cassert>
#include "criticalscope.h"
#include "mutexlock.h"
#include "patternexception.h"
#include "defaultpolicy.h"
#include "uncopyable.h"

namespace g2
	{

	//-----------------------------------------------------------------------------------------//
	// Singleton
	template
	<
		class Product,
		template < class > class CreationPolicy = default_policy::CreationPolicy,
		template < class > class DestructionPolicy = default_policy::DestructionPolicy
		>
	class Singleton :private g2::Uncopyable
		{
		public:
			static Product* Instance();
			static void Destroy();

		protected:
			Singleton();
			virtual ~Singleton(){};

			static Product *instance_;
			static g2::MutexLock mLock_;
		};

//-----------------------------------------------------------------------------------------//
template
<
	class Product,
	template < class > class CreationPolicy,
	template < class > class DestructionPolicy
	>
Singleton< Product, CreationPolicy, DestructionPolicy >::Singleton()
	{
	assert( instance_ == NULL && "instance is not NULL" );
	/*
	  if( instance_ != NULL )
	  {
	  throw PatternException( "Singleton Constructor has been called once" );
	  }
	*/
	}

	//-----------------------------------------------------------------------------------------//
	template
	<
		class Product,
		template < class > class CreationPolicy,
		template < class > class DestructionPolicy
		>
	Product* Singleton< Product, CreationPolicy, DestructionPolicy >::Instance()
			{
			if( instance_ == NULL )
				{
				CriticalScope< MutexLock > cScope( mLock_ );
				if( instance_ == NULL )
					{
					instance_ = CreationPolicy< Product >::Create();
					}
				}

			return instance_;
			}

//-----------------------------------------------------------------------------------------//
template
<
	class Product,
	template < class > class CreationPolicy,
	template < class > class DestructionPolicy
	>
void Singleton< Product, CreationPolicy, DestructionPolicy >::Destroy()
	{
	if( instance_ != NULL )
		{
		DestructionPolicy< Product >::Destroy( instance_ );
		instance_ = NULL;
		}
	}
	//-----------------------------------------------------------------------------------------//
	template
	<
		class Product,
		template < class > class CreationPolicy,
		template < class > class DestructionPolicy
		>
	Product* Singleton< Product, CreationPolicy, DestructionPolicy >::instance_ = NULL;

//-----------------------------------------------------------------------------------------//
template
<
	class Product,
	template < class > class CreationPolicy,
	template < class > class DestructionPolicy
	>
MutexLock Singleton< Product, CreationPolicy, DestructionPolicy >::mLock_;
	}


