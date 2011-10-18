#pragma once
#include <sys/types.h>
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
			inline void Remap( off_t offset_begin, off_t size );
			inline void Unmap();

			inline void* GetBegin() const { return begin_; }
			inline void* GetEnd() const { return end_; }
			inline void* GetReadPosition() const { return rpos_; }
			inline void* GetWritePosition() const { return wpos_; }
			inline void SetReadPosition( off_t offset ){ rpos_ = begin_ + offset; }
			inline void SetWritePosition( off_t offset ){ wpos_ = begin_ + offset; }
			inline void AddReadCompletionSize( off_t size ){ rpos_ += size; }
			inline void AddWriteCmpletionSize( off_t size ){ wpos_ += size; }

		private:
			void *begin_;
			void *end_;
			void *rpos_;
			void *wpos_;
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
		void *p = mmap( NULL, size, prot, MAP_SHARED, fd, offset_begin );
		if( p == MAP_FAILED )
			{
			throw Exception( "mmap() failed" );
			}
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Remap( off_t offset_begin, off_t size )
		{
		void *p = mremap( begin
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Unmap()
		{
		}
	}
