#pragma once

#include <vector>
#include "networkexception.h"
#include "uncopyable.h"

namespace g2
	{
	using namespace std;
		
	template < class T >
	class SocketHashMap :private g2::Uncopyable< SocketHashMap< T > >
		{
		public:
			SocketHashMap();
			~SocketHashMap();

			T& Add( int sock );
			template < class A0 >
			T& Add( int sock, const A0 &a0 );
			template < class A0,class A1 >
			T& Add( int sock, const A0 &a0,const A1 &a1 );
			template < class A0,class A1,class A2 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2 );
			template < class A0,class A1,class A2,class A3 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3 );
			template < class A0,class A1,class A2,class A3,class A4 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4 );
			template < class A0,class A1,class A2,class A3,class A4,class A5 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5 );
			template < class A0,class A1,class A2,class A3,class A4,class A5,class A6 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6 );
			template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7 );
			template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7,const A8 &a8 );
			template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9 >
			T& Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7,const A8 &a8,const A9 &a9 );
				
			void Remove( int sock );
			bool HasRegistered( int sock );				

			T& Get( int sock );
			bool Get( int sock, T &object );
			inline T& operator [] ( int sock );
			void Clear();
				
			size_t GetCapacity() const;

		private:
			struct ObjectHolder
				{
				public:
					bool isUsed;
					char object[ sizeof( T ) ];
				};

			ObjectHolder* InitializeChunk();
			void FinalizeChunk( ObjectHolder *chunk );
			ObjectHolder& AddChunk( int sock );
			void Destruct( ObjectHolder &chunk );
			inline bool IsValidSocket( int sock );
				
			vector< ObjectHolder* > table_;

			static const size_t DIVISOR_SIZE_ = 101;
		};

	//-----------------------------------------------------------------------------------------//
	template < class T >
	SocketHashMap< T >::SocketHashMap()
		{
		ObjectHolder *Chunk = InitializeChunk();
		table_.push_back( Chunk );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	SocketHashMap< T >::~SocketHashMap()
		{
		typename vector< ObjectHolder* >::iterator head = table_.begin();
		typename vector< ObjectHolder* >::iterator foot = table_.end();
			
		for(; head != foot; ++head )
			{
			FinalizeChunk( *head );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T& SocketHashMap< T >::Add( int sock )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T;
		holder.isUsed = true;

		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3 );
		holder.isUsed = true;
			
		return *place;
		}
		
	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4,class A5 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4,a5 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4,class A5,class A6 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4,a5,a6 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4,a5,a6,a7 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7,const A8 &a8 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4,a5,a6,a7,a8 );
		holder.isUsed = true;
			
		return *place;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	template < class A0,class A1,class A2,class A3,class A4,class A5,class A6,class A7,class A8,class A9 >
	T& SocketHashMap< T >::Add( int sock, const A0 &a0,const A1 &a1,const A2 &a2,const A3 &a3,const A4 &a4,const A5 &a5,const A6 &a6,const A7 &a7,const A8 &a8,const A9 &a9 )
		{
		ObjectHolder &holder = AddChunk( sock );
		T *place = reinterpret_cast< T* >( holder.object );

		if( holder.isUsed == true )
			{
			throw NetworkException( "SocketHashMap< T >::Add() failed. collision happend" );
			}
			
		place = new ( place ) T( a0,a1,a2,a3,a4,a5,a6,a7,a8,a9 );
		holder.isUsed = true;
			
		return *place;
		}

		
	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SocketHashMap< T >::Remove( int sock )
		{
		if( IsValidSocket( sock ) == false )
			{
			return;
			}

		ObjectHolder &holder = table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ];
		if( holder.isUsed == true )
			{
			Destruct( holder );
			}

		// no need to delete. this class d-tor is responsible for deleting memory
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	bool SocketHashMap< T >::HasRegistered( int sock )
		{
		if( IsValidSocket( sock ) == true &&
			table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ].isUsed == true )
			{
			return true;
			}
		
		return false;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	bool SocketHashMap< T >::Get( int sock, T &object )
		{
		if( IsValidSocket( sock ) == false ||
			table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ].isUsed == false )
			{
			return false;
			}

		object = *( reinterpret_cast< T* >( table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ].object ) );

		return true;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T& SocketHashMap< T >::Get( int sock )
		{
		if( IsValidSocket( sock ) == false ||
			table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ].isUsed == false )
			{
			throw NetworkException( "SocketHashMap< T >::Get() failed. invalid socket was passed" );
			}

		return *( reinterpret_cast< T* >( table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ].object ) );
		}	

	//-----------------------------------------------------------------------------------------//
	template < class T >
	T& SocketHashMap< T >::operator [] ( int sock )
		{
		return Get( sock );
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SocketHashMap< T >::Clear()
		{
		typename vector< ObjectHolder* >::iterator head = table_.begin();
		typename vector< ObjectHolder* >::iterator foot = table_.end();
			
		for(; head != foot; ++head )
			{
			ObjectHolder *Chunk = *head;
			for( size_t i = 0; i < DIVISOR_SIZE_; ++i )
				{
				Destruct( Chunk[i] );
				}
			}			
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	size_t SocketHashMap< T >::GetCapacity() const
		{
		return table_.size() * DIVISOR_SIZE_;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	typename SocketHashMap< T >::ObjectHolder* SocketHashMap< T >::InitializeChunk()
		{
		ObjectHolder *Chunk = new ObjectHolder [ DIVISOR_SIZE_ ];

		for( size_t i = 0; i < DIVISOR_SIZE_; ++i )
			{
			( Chunk + i )->isUsed = false;
			}

		return Chunk;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SocketHashMap< T >::FinalizeChunk( SocketHashMap< T >::ObjectHolder *chunk )
		{
		for( size_t i = 0; i < DIVISOR_SIZE_; ++i )
			{
			Destruct( chunk[i] );
			}

		delete [] chunk;
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	typename SocketHashMap< T >::ObjectHolder& SocketHashMap< T >::AddChunk( int sock )
		{
		unsigned int table_index = sock / DIVISOR_SIZE_;

		while( table_.size() < table_index + 1 )
			{
			table_.push_back( InitializeChunk() );
			}

		
		return table_[ sock / DIVISOR_SIZE_ ][ sock % DIVISOR_SIZE_ ];
		}

	//-----------------------------------------------------------------------------------------//
	template < class T >
	void SocketHashMap< T >::Destruct( ObjectHolder &chunk )
		{
		if( chunk.isUsed == true )
			{
			reinterpret_cast< T* >( chunk.object )->~T();
			chunk.isUsed = false;
			}
		}
		
	//-----------------------------------------------------------------------------------------//
	template < class T >
	bool SocketHashMap< T >::IsValidSocket( int sock )
		{
		if( sock < 0 ||
			table_.size() <= 0 ||
			table_.size()-1 < ( sock / DIVISOR_SIZE_ ) )
			{
			return false;
			}

		return true;
		}
	}


#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SocketHashMapTest

#include <stdio.h>

/*
<Makefile.am>
guts_SOURCES=\
/home/kizi/devel/gensinx/g2/src/sockethashmap.h\
/home/kizi/devel/gensinx/g2/src/sockethashmap.cpp
</Makefile.am>
 */

using namespace g2;
#include <boost/function.hpp>
#include <sstream>


class Dummy
	{	
	public:
		Dummy():id_(0)
			{
			puts( "Dummy();" );
			}
		Dummy( int a0 ):id_(a0)
			{
			printf( "Dummy( int a0 );\n" );
			}
		Dummy( int a0, int a1, int a2, int a3 ):id_(a0)
			{
			}
		
		~Dummy()
			{
			printf( "~Dummy() %d\n", id_ );
			}

		void F()
			{
			printf( "ID==%d\n", id_ );
			}

	private:
		size_t id_;
	};


struct EntryItem
	{
		int i;
		boost::function<bool()> f;
	};


int GutsEntrySocketHashMapTest( int, char** )
	{

	SocketHashMap< Dummy > shm;
	shm.Add( 17 );
	shm.Remove( 17 );
	shm.Remove( 17 );

	/*
	// normal test
	Dummy &d0 = shm.Add( 1 );
	d0.F();
	Dummy &dx = shm.Get( 1 );
	dx.F();

	// socket, arg
	shm.Add( 2, 1 );
	Dummy &d1 = shm.Get( 2 );
	d1.F();

	Dummy &d2 = shm[1];
	d2.F();

	// abnormal test
	try
		{
		shm.Add( 1 );
		}
	catch( NetworkException &E )
		{
		puts( E.GetMsg().c_str() );
		}

	try
		{
		shm.Remove( 3 );
		}
	catch( NetworkException &E )
		{
		puts( E.GetMsg().c_str() );
		}

	try
		{
		shm.Get( 3 );
		}
	catch( NetworkException &E )
		{
		puts( E.GetMsg().c_str() );
		}
	

	shm.Remove( 1 );
	try
		{
		shm.Get(1);
		}
	catch( NetworkException &E )
		{
		puts( E.GetMsg().c_str() );
		}
	*/

	return 0;
	}

#endif
#endif
