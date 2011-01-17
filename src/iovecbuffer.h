#pragma once
#include <stdint.h>
#include "uncopyable.h"

namespace g2
	{
	template < size_t ChunkSize >
	class IOVecBuffer
		{
		private:
			G2_MARK_UNCOPYABLE( IOVecBuffer<ChunkSize> );
			
			struct Chunk
				{
					size_t bufSize;
					uint8_t buffer[ ChunkSize ];
				};
			
		public:
			IOVecBuffer();
			explicit IOVecBuffer( size_t ringSize );
			~IOVecBuffer();
			
			uint8_t operator [] ( size_t byteIndex )
				{
				if( ringIndex_ < ringSize_ )
					{
					if( byteIndex < chunks_[ ringIndex_ ].bufSize )
						{
						return chunks_
						}
					}
				}
			
		private:
			struct Chunk *chunks_;
			size_t ringSize_;
			size_t ringIndex_;
		};
	}

