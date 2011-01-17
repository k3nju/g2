#pragma once

#define CORO(l) switch( int &line = l )						\
	  case -1:if(1)											\
			{												\
            E:												\
			break;											\
			} else case 0:

#define YIELD if(0){case __LINE__:;}else for(;;)if(line==__LINE__)goto E;else if((line=__LINE__))switch(1) case 1:
