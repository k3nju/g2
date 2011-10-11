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
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::OpenAdjust()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Read( char *buf, size_t size )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Write( const char *buf, size_t size )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Close()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void MappedFile::Flush()
		{
		}
	}