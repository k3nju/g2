#pragma once
#include <stdint.h>
#include "uncopyable.h"

namespace g2
	{
	// * Memory image.
	// |--block---------------------|
	// |[chunk][chunk][chunk][chunk]|
	
	class FixedSizeAllocator
		{
			G2_MAKR_UNCOPYABLE( FixedSizeAllocator );
			
		public:
			FixedSizeAllocator( size_t chunkSize );
			~FixedSizeAllocator();

			void AddBlock( void *block, size_t size );
			
			inline void* Allocate();
			inline void Free( void *chunk );
			
		private:
			uint8_t *head_;
			size_t chunkSize_;
		};

	//-----------------------------------------------------------------------------------------//
	void* FixedSizeAllocator::Allocate()
		{
		
		}
	}
