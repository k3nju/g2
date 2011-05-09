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
			
		private:
			typedef struct _chunk
				{
					struct _chunk *next;
					uint8_t data[1];
				} chunk_t;

			static const size_t CHUNK_HEADER_SIZE = sizeof( chunk_t ) - 1;
			
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
		chunk_t *ret = head_;
		head_ = head_->next;

		return ret;
		}

	//-----------------------------------------------------------------------------------------//
	void FixedSizeAllocator::Free( void *chunk )
		{
		chunk_t *newHead = (chunk_t*)( (uint8_t*)chunk + sizeof( chunk_t ) - 1 );
		SetNext( newHead, head_ );
		head_ = newHead;
		}
	}
