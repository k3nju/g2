#include "fixedsizeallocator.h"
#include <cassert>
#include "exception.h"
#include <stdio.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	FixedSizeAllocator::FixedSizeAllocator( size_t chunkSize )
		:head_( NULL ),
		 chunkSize_( CHUNK_HEADER_SIZE + chunkSize )
		{
		assert( chunkSize != 0 );
		}

	//-----------------------------------------------------------------------------------------//
	FixedSizeAllocator::~FixedSizeAllocator()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void FixedSizeAllocator::AddBlock( void *block, size_t size )
		{
		if( size < chunkSize_ )
			{
			return;
			}
		
		uint8_t *tmp = (uint8_t*)block;
		size_t count = size / chunkSize_;
		printf( "%d\n", count );
		
		for( size_t i = 0; i < count; ++i )
			{
			SetNext( tmp, tmp + chunkSize_ );
			printf( "%p %p\n", tmp, tmp + chunkSize_ );
			tmp = tmp + chunkSize_;
			}
		
		// if "block" is the initial block,
		// the "chunk_t.next" value of "tmp" which points to end of "block" will be NULL. (described as A)
		// otherwise, "chunk_t.next" value of "tmp" will be old head of block. (described as B)
		//
		// A.
		// |--block--------------------------------------|
		// |[chunk_t.next]->[chunk_t.next]->[chunk_t.next]->NULL|
		// ^->head_
		//
		// B.
		// _1. before updating linked list.
		// |--old block----------------------------------|
		// |[chunk_t.next]->[chunk_t.next]->[chunk_t.next]->NULL|
		// ^->head_
		// ^-------------------------------------------------
		//                                                  |
		// _2. after updating linked list.                  |
		// |--new block----------------------------------|  |--old block----------------------------------|
		// |[chunk_t.next]->[chunk_t.next]->[chunk_t.next]->|[chunk_t.next]->[chunk_t.next]->[chunk_t.next]->NULL|
		// ^->head_                                         ^-old head_
		//
		SetNext( tmp, head_ );
		head_ = (chunk_t*)block;
		}
	}
