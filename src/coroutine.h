#pragma once

#define G2_COROUTINE(l)							\
	switch( int &line = l )						\
	  case -1:									\
		if(1)									\
			{;G2_COROUTINE_OUT:break;}			\
		else case 0:

#define G2_YIELD														\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if(line==__LINE__)goto G2_COROUTINE_OUT;					\
			else if((line=__LINE__)) switch(1)case 1:
