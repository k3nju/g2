#pragma once

#define G2_COROUTINE(l)							\
	int *__FILE__##__LINE__ = l;				\
	switch( *__FILE__##__LINE__ )				\
	  case -1:									\
		if(1)									\
			{;G2_COROUTINE_OUT:break;}			\
		else case 0:

#define G2_YIELD_ONCE													\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if(*__FILE__##__LINE__==__LINE__)goto G2_COROUTINE_OUT;		\
			else if((*__FILE__##__LINE__=__LINE__)) switch(1)case 1:

#define G2_YIELD														\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if((*__FILE__##__LINE__=__LINE__)) switch(1)case 1:


#define G2_COROUTINE_TLS							\
	static __thread int __FILE__##__LINE__ = 0;		\
	switch( __FILE__##__LINE__ )					\
	  case -1:										\
		if(1)										\
			{;G2_COROUTINE_OUT:break;}				\
		else case 0:

#define G2_YIELD_ONCE_TLS												\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if(__FILE__##__LINE__==__LINE__)goto G2_COROUTINE_OUT;		\
			else if((__FILE__##__LINE__=__LINE__)) switch(1)case 1:

#define G2_YIELD_TLS													\
	if(0){case __LINE__:;}												\
	else																\
		for(;;)															\
			if((__FILE__##__LINE__=__LINE__)) switch(1)case 1:

