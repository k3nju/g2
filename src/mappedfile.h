#pragma once
#include "uncopyable.h"

namespace g2
	{
	class MappedFile
		{
			G2_MARK_UNCOPYABLE( MappedFile );
			
		public:
			enum FLAGS
				{
				READ      = O_RDONLY,
				WRITE     = O_WRONLY,
				READWRITE = O_RDWR
				};
			
			MappedFile( const char *filename, FLAGS flags );
			~MappedFile();

			ssize_t Read( void *buf, size_t size );
			ssize_t Write( void *buf, size_t size );
			void Close();
			void Flush();
			
			size_t Foward( size_t bytes );
			size_t Backward( size_t bytes );
			
		};
	}
