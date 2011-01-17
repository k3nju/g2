#pragma once
#include <stddef.h>
#include "uncopyable.h"

namespace g2
	{
	template < class OperationType >
	class ScopedOperation :private g2::Uncopyable
		{
		public:
			ScopedOperation( OperationType &op );
			template < class T1 >
			ScopedOperation( OperationType &op, const T1 &t1 );
			template < class T1, class T2 >
			ScopedOperation( OperationType &op, const T1 &t1, const T2 &t2 );
			template < class T1, class T2, class T3 >
			ScopedOperation( OperationType &op, const T1 &t1, const T2 &t2, const T3 &t3 );
			~ScopedOperation();

			void Cancel();

		private:
			OperationType *op_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	ScopedOperation< OperationType >::ScopedOperation( OperationType &op ):op_( &op )
		{
		op_->StartScopedOperation();
		}

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	template < class T1 >
	ScopedOperation< OperationType >::ScopedOperation( OperationType &op, const T1 &t1 ):op_( &op )
		{
		op_->StartScopedOperation( t1 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	template < class T1, class T2 >
	ScopedOperation< OperationType >::ScopedOperation( OperationType &op, const T1 &t1, const T2 &t2 ):op_( &op )
		{
		op_->StartScopedOperation( t1, t2 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	template < class T1, class T2, class T3 >
	ScopedOperation< OperationType >::ScopedOperation( OperationType &op, const T1 &t1, const T2 &t2, const T3 &t3 ):op_( &op )
		{
		op_->StartScopedOperation( t1, t2, t3 );
		}

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	ScopedOperation< OperationType >::~ScopedOperation()
		{
		if( op_ != NULL )
			{
			op_->EndScopedOperation();
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class OperationType >
	void ScopedOperation< OperationType >::Cancel()
		{
		op_ = NULL;
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__ScopedOpTest

#include <stdio.h>

struct Test
	{
	public:
		void StartScopedOperation(){puts("1");}
		void StartScopedOperation( int i ){printf( "%d\n", i );}
		void StartScopedOperation( int i, const char *s ){ printf( "%d %s\n", i, s ); }
		void EndScopedOperation(){puts( "end" );};
	};

int GutsEntryScopedOpTest(int,char**)
	{

	{
	Test t;
	g2::ScopedOperation< Test > s( t );
	}

	{
	Test t;
	g2::ScopedOperation< Test > s( t, 1 );
	}

	{
	Test t;
	g2::ScopedOperation< Test > s( t, 1, "test" );
	}
	
	return 0;
	}

#endif
#endif

