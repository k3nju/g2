#pragma once
#include <stddef.h>
#include <stdint.h>
#include "uncopyable.h"

namespace g2
	{
	// * Memory image.
	// |--block---------------------|
	// |[chunk][chunk][chunk][chunk]|
	
	class FixedSizeAllocator
		{
			G2_MARK_UNCOPYABLE( FixedSizeAllocator );
			
		public:
			FixedSizeAllocator( size_t chunkSize );
			~FixedSizeAllocator();

			void AddBlock( void *block, size_t size );
			
			inline void* Allocate();
			inline void Free( void *chunk );
			inline bool IsEmpty();
			
		private:
			typedef struct _chunk
				{
					struct _chunk *next;
				} chunk_t;
			
			inline void SetNext( void *chunk, void *nextChunk )
				{
				((chunk_t*)chunk)->next = (chunk_t*)nextChunk;
				}
			
			chunk_t *head_;
			size_t chunkSize_;
		};

	//-----------------------------------------------------------------------------------------//
	void* FixedSizeAllocator::Allocate()
		{
		if( head_ == NULL )
			{
			return NULL;
			}
		
		chunk_t *ret = head_;
		head_ = head_->next;

		return ret;
		}

	//-----------------------------------------------------------------------------------------//
	void FixedSizeAllocator::Free( void *chunk )
		{
		if( chunk == NULL )
			{
			return;
			}
		
		chunk_t *newHead = (chunk_t*)chunk;
		SetNext( newHead, head_ );
		head_ = newHead;
		}

	//-----------------------------------------------------------------------------------------//
	bool FixedSizeAllocator::IsEmpty()
		{
		return head_ == NULL;
		}
	
	}
