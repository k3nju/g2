#pragma once
#include <fcntl.h>
#include <string>
#include "uncopyable.h"

namespace g2
	{
	class MappedFile
		{
			G2_MARK_UNCOPYABLE( MappedFile );
			
		public:
			enum OPEN_FLAGS
				{
				CREATE = O_CREATE,
				TRUNC = O_TRUNC,
				READ = O_RDONLY,
				WRITE = O_WRONLY | O_CREAT,
				READWRITE = O_RDWR | O_CREAT
				};
			
			MappedFile( const std::string &filename, OPEN_FLAGS flags, mode_t mode, off_t pageSize );
			~MappedFile();

			void Open();
			void Close();
			off_t Read( char *buf, off_t size );
			void Write( const char *buf, off_t size );
			void Flush();
			
		private:
			int Conv2Prot( OPEN_FLAGS flags );
			
			struct FileDescriptor
				{
				public:
					inline FileDescriptor():fd( -1 ){};
					bool IsOpened() const { return fd != -1; }
					void Open( const char *file, int flags, mode_t mode );
					void Close();
					void Sync();
					void Truncate( off_t size );
					struct stat GetStat() const;

					int fd;
				};
			
			struct MappedRange
				{
				public:
					MappedRange(){ begin = end = rpos = wpos = NULL; }
					~MappedRange();
					
					void Map( int fd, int prot, off_t begin, off_t size );
					void Remap( off_t begin, off_t size );
					void Unmap();
					off_t GetSize() const;
					off_t GetWritableSize() const;
					off_t GetReadableSize() const;
					void AddReadCompletionSize( off_t size );
					void AddWriteCompletionSize( off_t size );
					
					void *begin;
					void *end;
					void *rpos;
					void *wpos;
				};

			std::string filename_;
			int openFlags_;
			mode_t mode_;
			off_t pageSize_;
			off_t fileSize_;

			FileDescriptor fd_;
			MappedRange mrange_;
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__MappedFileTest

#include <cassert>
#include <cstring>
#include "exception.h"

using namespace g2;
using namespace std;

int GutsEntryMappedFileTest( int, char** )
	{
	try
		{
		char tmp[1024];
		memset( tmp, '\0', sizeof( tmp ) );
		MappedFile mfile;

		mfile.Open( "/tmp/aaa", MappedFile::READWRITE, 0644, 4 );
		mfile.Read( tmp, 1024 );
		puts( tmp );
		return 0;
		assert( mfile.Write( "ABC", 3 ) == 3 );
		assert( mfile.Backward( 3 ) == 3 );
		assert( mfile.Read( tmp, 3 ) == 3 );
		assert( memcmp( tmp, "ABC", 3 ) == 0 );
		assert( mfile.SetOffset( 0 ) == 0 );

		assert( mfile.Write( "DEFG", 4 ) == 4 );
		assert( mfile.Read( tmp, 4 ) == 0 );
		assert( mfile.Backward( 4 ) == 4 );
		assert( mfile.Read( tmp, 4 ) == 4 );
		assert( memcmp( tmp, "DEFG", 4 ) == 0 );
		assert( mfile.SetOffset( 0 ) == 0 );
		
		assert( mfile.Write( "HIJKL", 5 ) == 4 );
		assert( mfile.Read( tmp, 5 ) == 0 );
		assert( mfile.Backward( 5 ) == 4 );
		assert( mfile.Read( tmp, 5 ) == 4 );
		assert( memcmp( tmp, "HIJK", 4 ) == 0 );
		
		assert( mfile.SetOffset( 0 ) == 0 );
		assert( mfile.Forward( 3 ) == 3 );
		assert( mfile.SetOffset( 0 ) == 0 );
		assert( mfile.Forward( 4 ) == 4 );
		assert( mfile.SetOffset( 0 ) == 0 );
		assert( mfile.Forward( 5 ) == 4 );
		assert( mfile.GetOffset() == 4 );
		mfile.Extend( 1 );
		
		assert( mfile.Write( "M", 1 ) == 1 );
		assert( mfile.GetOffset() == 5 );
		//mfile.Flush();
		/**/
		
		mfile.Close();
		}
	catch( Exception &E )
		{
		E.Output();
		}

	return 0;
	}

#endif
#endif
