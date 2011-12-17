#pragma once

#define G2_COROUTINE(l)							\
	int *__FILE__##__FILE__ = l;				\
	switch( *__FILE__##__FILE__ )				\
	  case -1:									\
		if(1)									\
			{;G2_COROUTINE_OUT:break;}			\
		else case 0:

#define G2_YIELD														\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if(*__FILE__##__FILE__==__LINE__)goto G2_COROUTINE_OUT;		\
			else if((*__FILE__##__FILE__=__LINE__)) switch(1)case 1:
