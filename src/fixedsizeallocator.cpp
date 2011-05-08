#include "fixedsizeallocator.h"
#include <stdlib.h>
#include <unistd.h>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	FixedSizeAllocator::FixedSizeAllocator( size_t chunkSize )
		:head_( NULL ),
		 chunkSize_( chunkSize );
		{
		assert( chunkSize != 0 );
		}

	//-----------------------------------------------------------------------------------------//
	void FixedSizeAllocator::AddBlock( void *block, size_t size )
		{
		if( size < chunkSize_ )
			{
			return;
			}
		
		uint8_t *tmp = (uint8_t*)block;
		size_t chunkCount = size / chunkSize_;

		for( size_t i = 0; i < chunkCount; ++i )
			{
			tmp
			}
		}
	}
