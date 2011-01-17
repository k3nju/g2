#pragma once

#include <unistd.h>
#include <string>
#include <vector>
#include "uncopyable.h"
#include "range.h"

namespace g2
	{
	class SysUtility
		{
			G2_MARK_UNCOPYABLE( SysUtility );
			
		public:
			class MemoryMap
				{
					G2_MARK_UNCOPYABLE( MemoryMap );

				public:
					struct RangeInfo
						{							
							RangeInfo()
								:range(),
								 isReadable( false ),
								 isWritable( false ),
								 isExecutable( false ),
								 isShared( false ),
								 isPrivate( false ),
								 inode( 0 ),
								 pathName()
								{
								}
							
							// memory range
							Range range;
							
							// permissions
							bool isReadable;
							bool isWritable;
							bool isExecutable;
							bool isShared;
							bool isPrivate;

							// offset
							unsigned long long offset;
							
							// inode
							unsigned long long inode;

							// pathname
							std::string pathName;
						};
					
					MemoryMap( pid_t ipd );
					~MemoryMap();

					void Load();

					RangeInfo GetInfo( void *addr ) const;
					RangeInfo operator [] ( void *addr ) const;
					
				private:
					void ParseLine( const char *cLine );
					
					pid_t pid_;
					std::vector< void* > memMap_;
					
				public:
					static void Debug()
						{
						pid_t pid = getpid();
						MemoryMap mm( pid );
						}
				};
		};
	}

#ifdef GENSIN__GUTS
#ifdef GENSIN__GUTS__ID__SysUtilTest

using namespace g2;

int GutsEntrySysUtilTest(int,char**)
	{
	SysUtility::MemoryMap::Debug();

	return 0;
	}

#endif
#endif
