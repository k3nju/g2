#include "mappedfile.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <algorithm>
#include <cstring>
#include <cassert>
#include "exception.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	MappedFile::MappedFile( const std::string &filename, OPEN_FLAGS flags, mode_t mode, off_t mapRangeSize )
		:filename_( filename ),
		 openFlags_( (int)flags ),
		 prot_( Conv2Prot( flags ) ),
		 mode_( mode ),
		 mapSize_( mapRangeSize_ ),
		 fd_()
		{
		}

	//-----------------------------------------------------------------------------------------//
	MappedFile::~MappedFile()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Open()
		{
		fd_.Open( filename.c_str(), flags_, mode_ );
		fileSize_ = fd_.GetFileSize();
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Close()
		{
		Unmap();
		fd_.Close();
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Map( off_t begin )
		{
		mrange_.Map( fd_.fd, prot_, begin, mapSize_ );
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Unmap()
		{
		mrange_.Unmap();
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Read( char *buf, size_t size )
		{
		size_t i = 0;
		while( size > 0 )
			{
			if( mrange.GetLeftoverSize() == 0 )
				{
				off_t next = mrange.offset_begin + mapSize_;
				mrange.Unmap();
				mrange.Map( fd_.fd, prot_, next, mapSize_ );
				}
			
			size_t sizeRead = mrange.Read( buf + i, size );
			i += sizeRead;
			size -= sizeRead;
			}
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Write( const char *buf, size_t size )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Flush()
		{
		fd_.Sync();
		}
	}
