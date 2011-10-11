#pragma once
#include <fcntl.h>
#include "uncopyable.h"

namespace g2
	{
	class MappedFile
		{
			G2_MARK_UNCOPYABLE( MappedFile );
			
		public:
			enum OPEN_FLAGS
				{
				READ = O_RDONLY | O_TRUNC,
				WRITE = O_WRONLY | O_CREAT | O_TRUNC,
				READWRITE = O_RDWR | O_CREAT | O_TRUNC
				};
			
			MappedFile();
			~MappedFile();

			void Open( const char *filename, OPEN_FLAGS flags, mode_t mode, off_t mapSize );
			size_t Read( char *buf, size_t size );
			size_t Write( const char *buf, size_t size );
			void Close();
			void Flush();
			void Extend( size_t size );

			off_t SetOffset( off_t offset );
			off_t GetOffset() const;
			size_t Forward( size_t size );
			size_t Backward( size_t size );

		private:
			int fd_;
			char *base_;
			char *end_;
			char *pos_;
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
		MappedFile mfile;

		mfile.Open( "/tmp/aaa", MappedFile::READWRITE, 0644, 4 );
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
