#ifndef GENSIN_DaemonH
#define GENSIN_DaemonH

namespace g2
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

