#include "sysutility.h"
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include "utility.h"
#include "exception.h"

#include <cstring>
#include "debugutility.h"

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	SysUtility::MemoryMap::MemoryMap( pid_t pid )
		:pid_( pid )
		{
		Load();
		}

	//-----------------------------------------------------------------------------------------//
	SysUtility::MemoryMap::~MemoryMap()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void SysUtility::MemoryMap::Load()
		{
		using namespace std;
		string filePath = "/proc/" + Utility::Convert2String( pid_ ) + "/maps";
		ifstream in;
		in.open( filePath.c_str(), std::ios::in );
		
		if( in.is_open() == false )
			{
			return;	
			}

		try
			{
			while( in.eof() == false )
				{
				char buf[1000];
				memset( buf, '\0', sizeof( buf ) );
				//08048000-0804c000 r-xp 00000000 08:01 236208     /usr/lib/scim-1.0/scim-helper-launcher
				
				in.getline( buf, sizeof( buf ) - 1 );
				ParseLine( buf );
				}
			}
		catch( ... )
			{
			}
		
		in.close();
		}
	
	//-----------------------------------------------------------------------------------------//
	void SysUtility::MemoryMap::ParseLine( const char *cLine )
		{
		RangeInfo ri;
		std::string line( cLine );
		size_t pos = 0;

		// memory range start
		pos = line.find( "-" );
		
		if( pos == std::string::npos )
			return;
		ri.range.head = (char*)boost::lexical_cast< g2::HexTo<void*> >( line.substr( 0, pos ).c_str() ).value;
		

		// memory range end
		pos += 1;
		line = line.substr( pos );
		pos = line.find( " " );
		if( pos == std::string::npos )
			return;
		
		ri.range.foot = (char*)boost::lexical_cast< g2::HexTo<void*> >( line.substr( 0, pos ).c_str() ).value;

		// permissions
		pos += 1;
		line = line.substr( pos );
		pos = line.find( " " );
		if( pos == std::string::npos )
			return;

		if( line[0] == 'r' )
			ri.isReadable = true;
		if( line[1] == 'w' )
			ri.isWritable = true;
		if( line[2] == 'x' )
			ri.isExecutable = true;
		if( line[3] == 's' )
			ri.isShared = true;
		if( line[3] == 'p' )
			ri.isPrivate = true;
		
		// offset
		pos += 1;
		line = line.substr( pos );
		pos = line.find( " " );
		ri.offset = boost::lexical_cast< g2::HexTo< unsigned long long > >( line.substr( 0, pos ).c_str() );
		
		// device
		pos += 1;
		line = line.substr( pos );
		pos = line.find( " " );

		// inode
		pos += 1;
		line = line.substr( pos );
		pos = line.find( " " );
		ri.inode = boost::lexical_cast< g2::HexTo< unsigned long long > >( line.substr( 0, pos ).c_str() );
		
		// path
		pos = line.rfind( " " );
		ri.pathName = line.substr( pos + 1 );
		if( ri.pathName.size() == 0 )
			{
			ri.pathName = "None";
			}
		}

	//-----------------------------------------------------------------------------------------//
	SysUtility::MemoryMap::RangeInfo void SysUtility::MemoryMap::GetInfo
	}

