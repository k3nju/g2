#pragma once
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/mman.h>
#include "exception.h"
#include "uncopyable.h"

namespace g2
	{
	struct MappedView
		{
		private:
			G2_MARK_UNCOPYABLE( MappedView );
			
		public:
			inline MappedView();
			inline ~MappedView();
			
			inline void Map( int fd, int prot, off_t offset_begin, off_t size );
			inline void Remap( off_t size );
			inline void Unmap();

			inline uint8_t* GetBegin() const { return begin_; }
			inline uint8_t* GetEnd() const { return end_; }
			inline uint8_t* GetReadPosition() const { return rpos_; }
			inline uint8_t* GetWritePosition() const { return wpos_; }
			inline void SetReadPosition( off_t offset ){ rpos_ = begin_ + offset; }
			inline void SetWritePosition( off_t offset ){ wpos_ = begin_ + offset; }
			inline void AddReadCompletionSize( off_t size ){ rpos_ += size; }
			inline void AddWriteCmpletionSize( off_t size ){ wpos_ += size; }

		private:
			uint8_t *begin_;
			uint8_t *end_;
			uint8_t *rpos_;
			uint8_t *wpos_;
		};

	//-----------------------------------------------------------------------------------------//
	MappedView::MappedView()
		:begin_( NULL ),
		 end_( NULL ),
		 rpos_( NULL ),
		 wpos_( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	MappedView::~MappedView()
		{
		if( begin_ != NULL )
			{
			try
				{
				Unmap();
				}
			catch( ... )
				{
				}
			}
		}
	
	//-----------------------------------------------------------------------------------------//
	void MappedView::Map( int fd, int prot, off_t offset_begin, off_t size )
		{
		uint8_t *p = mmap( NULL, size, prot, MAP_SHARED, fd, offset_begin );
		if( p == MAP_FAILED )
			{
			throw Exception( "mmap() failed", errno );
			}

		begin_ = p;
		end_ = p + size;
		rpos_ = p;
		wpos_ = p;
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Remap( off_t size )
		{
		uint8_t *p = mremap( begin_,  end_ - begin_, size, MREMAP_MAYMOVE );
		if( p == MAP_FAILED )
			{
			throw Exception( "mremap() failed", errno );
			}
		
		begin_ = p;
		end_ = p + size;
		rpos_ = p;
		wpos_ = p;
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Unmap()
		{
		if( munmap( begin_, end_ - begin_ ) == -1 )
			{
			throw Exception( "munmap() failed", errno );
			}
		}
	}
