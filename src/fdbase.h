#pragma once

#include <sys/types.h>
#include "uncopyable.h"

namespace g2
	{
	class FDBase
		{
			G2_MARK_UNCOPYABLE( FDBase );
			
		public:
			FDBase();
			explicit FDBase( int fd );
			~FDBase();
			
			inline void ResetFD( int fd ){ fd_ = fd; };
			inline int GetFD() const { return fd_; };

			virtual ssize_t Read( void *buf, size_t size );
			virtual ssize_t Write( void *buf, size_t size );
			virtual int Close();

		private:
			int fd_;
		};
	}

