#ifndef GENSIN_DaemonH
#define GENSIN_DaemonH

//#include "uncopyable.h"

namespace g2 //:private g2::Uncopyable
	{
	class Daemon
		{
		public:
			explicit Daemon( const char *chdir );
			~Daemon();

			bool BecomeDaemon();
			int RedirectSTDIN( int in );
			int RedirectSTDOUT( int out );
			int RedirectSTDERR( int err );
			
		private:
			const char *chdir_;
		};
	}

#endif

