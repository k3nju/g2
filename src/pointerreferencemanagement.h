#pragma once

#include <cstddef>
#include <cassert>
#include "uncopyable.h"

namespace g2
	{
	// this class must be instantiated on heap
	template < class LockingPolicy >
	class PointerReferenceManagement :private g2::Uncopyable
		{
		public:
			static PointerReferenceManagement< LockingPolicy >* Create( void * const pointer );
				
			void AddReference();
			template < class T > void RemoveReference();
				
			void* GetPointer() const;
				
		private:
			PointerReferenceManagement( void * const pointer );
				
			void * const pointer_;
			unsigned int count_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class LockingPolicy >
	PointerReferenceManagement< LockingPolicy >* PointerReferenceManagement< LockingPolicy >::Create( void * const pointer )
		{		
		return new PointerReferenceManagement< LockingPolicy >( pointer );
		}
		
	//-----------------------------------------------------------------------------------------//
	template < class LockingPolicy >
	PointerReferenceManagement< LockingPolicy >::PointerReferenceManagement( void * const pointer )
		:pointer_( pointer ), count_( 0 )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class LockingPolicy >
	void PointerReferenceManagement< LockingPolicy >::AddReference()
		{
		++count_;
		}
				
	//-----------------------------------------------------------------------------------------//
	template < class LockingPolicy >
	void* PointerReferenceManagement< LockingPolicy >::GetPointer() const
		{
		return pointer_;
		}
		
	//-----------------------------------------------------------------------------------------//
	template < class LockingPolicy >
	template < class T >
	void PointerReferenceManagement< LockingPolicy >::RemoveReference()
		{
		--count_;
		if( count_ == 0 )
			{
			delete static_cast< T* >( pointer_ );
			delete this;
			}
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__PointerReferenceManagementTest

#include "nulllock.h"

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devs/src/pointerreferencemanagement.h	\
/home/kizi/devs/src/pointerreferencemanagement.cpp
</Makefile.am>
 */

#include <stdio.h>
using namespace g2;
using namespace g2;
class Test;

typedef PointerReferenceManagement< NULLLock > PRMType;

class Test
	{
	public:
		Test(){ puts( "Test()" ); };
		~Test(){ puts( "~Test()" ); };

		void F(){ puts( "F" ); };
	};

	
int GutsEntryPRM( int, char** )
	{
	PRMType *PRM = PRMType::Create( new Test() );
	PRM->AddReference();
	PRM->AddReference();

	PRM->RemoveReference< Test >();

	static_cast< Test* >( PRM->GetPointer() )->F();

	PRM->RemoveReference< Test >();

	}


#endif
#endif
