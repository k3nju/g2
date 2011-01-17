#pragma once

namespace g2
	{
<<<<<<< HEAD:src/threadpool2.h
	template < class WorkerType >
	class ThreadPool2 :private g2::Uncopyable
		{
		public:
			typedef WorkerType worker_t;
			typedef typename worker_t::data_t data_t;
			typedef boost::shared_ptr< worker_t > worker_ptr_t;
			
			explicit ThreadPool2( size_t threadCount );
			~ThreadPool2();

			
=======
	//-----------------------------------------------------------------------------------------//
	template < class T >
	class Channel
		{
		public:
			void Enqueue( const T &t )
				{
				scoped_lock_t locked( lock_ );
				deq_.push_back( t );
				}

			bool Dequeue( T &t )
				{
				scoped_lock_t locked( lock_ );
				while( isEnd_ == false && deq_.empty() )
					{
					cond_.Wait();
					}
				
				if( isEnd_ == true )
					{
					return false;
					}
				
				t = deq_.front();
				deq_.pop();
				
				return true;
				}

			void StartScopedOperation()
				{
				__sync_add_and_fetch( &useCount_, 1 );
				}

			void EndScopedOperation()
				{
				__sync_sub_and_fetch( &useCount_, 1 );
				}
			
		private:
			bool isEnd_;
			size_t useCount_;
			g2::MutexLock lock_;
			g2::ConditionValue cond_;
			std::deq< T > deq_;
		};

	//-----------------------------------------------------------------------------------------//
	class TaskThreadPool :private g2::Uncopyable
		{
		private:
			class TaskWorker :public g2::Threading
				{
				private:
					virtual int Thread( void *argv )
						{
						Channel *channel = (Channel*)argv;
						g2::ScopedOperation addUseCount( *channel );
						task_t task;
						
						while( channel->Dequeue( task ) == true )
							{
							task();
							}
						}
				};
			
		public:
			explicit TaskThreadPool( size_t initialCount )
				{
				for( size_t i = 0; i < initialCount; ++i )
					{
					workers_.push_back( worker_prt_t( new worker_t() ) );
					}
				}
			
			~TaskThreadPool()
				{
				End();
				}
			
			void Begin()
				{
				workers_::iterator head = workers_.begin();
				workers_::iterator foot = workers_.end();
				
				for(; head != foot; ++head )
					{
					(*head)->SetArgv( &channel_ );
					(*head)->Create();
					}
				}
			
			void End()
				{
				workers_::iterator head = workers_.begin();
				workers_::iterator foot = workers_.end();
				
				for(; head != foot; ++head )
					{
					(*head)->Join();
					}
				}

			void Enqueue( task_t &task )
				{
				channel_.Enqueue( task );
				}

		private:
			typedef std::list< worker_ptr_t > worker_list_t;
			worker_list_t workers_;
>>>>>>> f9bbd0dd94b837e7b3e6013cbe7905f46c5bb9f4:src/threadpool2.h
		};
	};
