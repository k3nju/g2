#pragma once

#include <signal.h>
#include <pthread.h>

namespace g2
	{
	class Sigset
		{
		public:
			Sigset();
			Sigset( const Sigset &rhs );
			const Sigset& operator = ( const Sigset &rhs );
			~Sigset();

			void Empty();
			void Fill();
			void Add( int signal_number );
			void Remove( int signal_number );
			bool IsMember( int signal_number );

			// TODO: inline attribute makes "undefined error".
			// because of compiled binary don't  have Get() symbol.
			// i don't know why this happens...
			/*inline*/ const sigset_t& Get() const;

			void SetMask();
			void AddMask();
			
		private:
			void Mask( int how );
			
			sigset_t sigset_;
		};
	}

