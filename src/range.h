#pragma once
#include <stdio.h>
#include <stddef.h>

namespace g2
	{
	struct Range
		{
			Range()
				:head( NULL ), foot( NULL )
				{
				};

			void Clear()
				{
				head = foot = NULL;
				}

			bool AreBothNULL() const
				{
				return head == NULL && foot == NULL;
				}
			
			bool operator == ( const Range &rhs ) const
				{
				return ( head == rhs.head ) && ( foot == rhs.foot );
				}

			bool operator != ( const Range &rhs ) const
				{
				return !( *this == rhs );
				}

			ptrdiff_t GetDiff() const
				{
				return foot - head;
				}
			
			const char *head;
			const char *foot;
		};
	}


