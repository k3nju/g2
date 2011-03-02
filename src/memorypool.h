#pragma once

#include <deque>
#include <list>
#include <algorithm>
#include <boost/shared_ptr.hpp>
#include "mutexlock.h"
#include "criticalscope.h"
#include "uncopyable.h"

namespace g2
	{
	template < class Type, class LockPolicy = g2::MutexLock >
	class MemoryPool :private g2::Uncopyable< MemoryPool< Type, LockPolicy > >
		{
			typedef MemoryPool< Type, LockPolicy > self_t;

		public:
			typedef std::list< Type* > chunk_list_t;
			typedef std::deque< Type* > chunk_deque_t;
			typedef boost::shared_ptr< Type > chunk_ptr_t;
			
			explicit MemoryPool( size_t size );
			~MemoryPool();

			chunk_ptr_t Acquire();
			void Restore( Type *chunk );

			void Allocate( size_t size );
			void Deallocate( size_t size );
			void Deallocate();

			size_t GetSize() const;
			size_t GetAvailableSize() const;

		protected:
			inline chunk_list_t& GetChunkList(){ return chunks_; };
			inline chunk_deque_t& GetChunkDeque(){ return availables_; };

		private:
			void AllocateBlocks( size_t size );
			
			class Restorer
				{
				public:
					Restorer( self_t *parent );
					void operator ()( Type *chunk );

				private:
					self_t *parent_;
				};
			
			chunk_list_t chunks_;
			chunk_deque_t availables_;

			static const size_t BLOCK_SIZE_ = 1;
			
			LockPolicy chunkLock_;
		};

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	MemoryPool< Type, LockPolicy >::MemoryPool( size_t size )
		{
		AllocateBlocks( size );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	MemoryPool< Type, LockPolicy >::~MemoryPool()
		{
		try
			{
			if( chunks_.empty() == false )
				{
				Deallocate();
				}
			}
		catch( ... )
			{
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	typename MemoryPool< Type, LockPolicy >::chunk_ptr_t MemoryPool< Type, LockPolicy >::Acquire()
		{
		CriticalScope< LockPolicy > locked( chunkLock_ );
		
		if( availables_.empty() == true )
			{
			AllocateBlocks( BLOCK_SIZE_ );
			}

		Type *chunk = availables_.front();
		availables_.pop_front();

		//printf( "Acquire %p %s\n", (void*)chunk, typeid( Type ).name() );

		return chunk_ptr_t( chunk, Restorer( this ) );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::Restore( Type *chunk )
		{
		CriticalScope< LockPolicy > locked( chunkLock_ );
		//printf( "Restore %p %s\n", (void*)chunk, typeid( Type ).name() );
		availables_.push_back( chunk );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::Allocate( size_t size )
		{
		CriticalScope< LockPolicy > locked( chunkLock_ );

		AllocateBlocks( size );
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::Deallocate( size_t size )
		{
		CriticalScope< LockPolicy > locked( chunkLock_ );

		size_t max = size > availables_.size() ? availables_.size() : size;
		for( size_t i = 0; i < max; ++i )
			{
			Type *chunk = availables_.front();
			availables_.pop_front();

			typename chunk_list_t::iterator iter = find( chunks_.begin(), chunks_.end(), chunk );
			if( iter != chunks_.end() )
				{
				chunks_.erase( iter );
				}

			delete chunk;
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::Deallocate()
		{
		CriticalScope< LockPolicy > locked( chunkLock_ );

		availables_.clear();

		typename chunk_list_t::iterator head = chunks_.begin();
		typename chunk_list_t::iterator foot = chunks_.end();
		
		for(; head != foot; ++head )
			{
			delete *head;
			}

		chunks_.clear();
		} 

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	size_t MemoryPool< Type, LockPolicy >::GetSize() const
		{
		CriticalScope< LockPolicy > locked( const_cast< LockPolicy& >( chunkLock_ ) );
		
		return chunks_.size();
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	size_t MemoryPool< Type, LockPolicy >::GetAvailableSize() const
		{
		CriticalScope< LockPolicy > locked( const_cast< LockPolicy& >( chunkLock_ ) );
		
		return availables_.size();
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::AllocateBlocks( size_t size )
		{
		for( size_t i = 0; i < size; ++i )
			{
			Type *chunk = new Type();
			chunks_.push_back( chunk );
			availables_.push_back( chunk );
			}
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	MemoryPool< Type, LockPolicy >::Restorer::Restorer( self_t *parent ):parent_( parent )
		{
		}

	//-----------------------------------------------------------------------------------------//
	template < class Type, class LockPolicy >
	void MemoryPool< Type, LockPolicy >::Restorer::operator ()( Type *chunk )
		{
		if( parent_ != NULL )
			{
			parent_->Restore( chunk );
			parent_ = NULL;
			}
		}
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__MemoryPoolTest

#include <algorithm>
#include <list>
#include <stdio.h>
#include <deque>
using namespace std;

/*
<Makefile.am>
here=/home/kizi/devel/g2x/src/g2
guts_SOURCES= \
$(here)/memorypool.h \
$(here)/memorypool.cpp
</Makefile.am>
*/

using namespace g2;

size_t I = 0;

class T
	{
	public:
		T():i_(0){};
		T( int i ):i_( i )
			{
			printf( "ctor:%d\n", i_ );
			}
		~T()
			{
			printf( "dtor:%d\n", i_ );
			}

		void F()
			{
			printf( "F():%d\n", i_ );
			}


		int i_;
	};

template < class T >
void PrintSize( T &t )
	{
	printf( "size=%d availables=%d\n", t.GetSize(), t.GetAvailableSize() );
	}

class CTest
	{
	public:
		
	};

int GutsEntryMemoryPoolTest( int, char** )
	{
	typedef MemoryPool< int > mp_t;
	typedef mp_t::chunk_ptr_t c_t;

	mp_t ma(1);
	c_t c = ma.Acquire();

	mp_t mb(1);
	c_t d = mb.Acquire();
	
	return 0;
	}

#endif
#endif


