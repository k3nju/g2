#pragma once
#include <sys/types.h>
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
			
			inline void Map( int fd, int prot, off_t begin, off_t size );
			inline void Remap( off_t begin, off_t size );
			inline void Unmap();
			
			void *begin;
			void *end;
			void *rpos;
			void *wpos;
		};

	//-----------------------------------------------------------------------------------------//
	MappedView::MappedView()
		:begin( NULL ),
		 end( NULL ),
		 rpos( NULL ),
		 wpos( NULL )
		{
		}

	//-----------------------------------------------------------------------------------------//
	MappedView::~MappedView()
		{
		if( begin != NULL )
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
	void MappedView::Map( int fd, int prot, off_t begin, off_t size )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Remap( off_t begin, off_t size )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedView::Unmap()
		{
		}
	}
