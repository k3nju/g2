#include "iodispatch.h"
#include <boost/bind.hpp>
#include <cassert>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	IODispatch::IODispatch()
		:inputEntries_(),
		 inputEntryLock_(),
		 outputEntries_(),
		 outputEntryLock_(),
		 errorEntries_(),
		 errorEntryLock_(),
		 inputEpoll_(),
		 outputEpoll_(),
		 stopFlag_( false ),
		 stopEvent_( 0 )
		{
		// register stop handler
		handler_t stopHandler = boost::bind( &IODispatch::StopHandler, this );
		Request( stopEvent_.GetFD(), event_type::READ, stopHandler );

		// register output epoll handler
		inputEpoll_.Register( outputEpoll_.GetFD(), event_type::READ );
		struct EntryItem &entry = inputEntries_.Add( outputEpoll_.GetFD() );
		handler_t outputEpollHandler = boost::bind( &IODispatch::OutputEpollHandler, this );
		SetHandler( entry, event_type::WRITE, outputEpollHandler );
		}

	//-----------------------------------------------------------------------------------------//
	IODispatch::~IODispatch()
		{
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Request( int socket, event_type::TYPE type, handler_t &handler )
		{
		// select entries and epoll
		entries_t *entries = NULL;		
		g2::Epoll *epoll = NULL;
		critical_scope_t cs;
		
/*		if( type & event_type::READ )
			{
			entries = &inputEntries_;
			epoll = &inputEpoll_;
			lock.SetLock( &inputEntryLock_ );
			}
		else if( type & event_type::WRITE )
			{
			entries = &outputEntries_;
			epoll = &outputEpoll_;
			lock.SetLock( &outputEntryLock_ );
			}
		else if( type & event_type::ERRORS )
			{
			entries = &errorEntries_;
			epoll = &inputEpoll_;
			lock.SetLock( &errorEntryLock_ );
			}
		else
			{
			assert( "unknown event type was passed" );
			}
*/

		SetMembersByType( type, entries, epoll, cs );

		// get or add EntryItem and set propear method for epoll
		struct EntryItem *entry = NULL;
		void (Epoll:: *epollRegister)(int,int) = NULL;
		
		if( entries->HasRegistered( socket ) == true )
			{
			entry = &( entries->Get( socket ) );
			epollRegister = &Epoll::UpdateEvents;
			}
		else
			{
			entry = &( entries->Add( socket ) );
			epollRegister = &Epoll::Register;
			}
		
		// set handler to item selected above
		SetHandler( *entry, type, handler );
		// register or update events
		(epoll->*epollRegister)( socket, entry->events );
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Request( int socket, event_type::TYPE type )
		{
		entries_t *entries = NULL;
		g2::Epoll *epoll = NULL;
		critical_scope_t cs;

		SetMembersByType( type, entries, epoll, cs );
		
		struct EntryItem &entry = entries->Get( socket );
		entry.events |= type;
		epoll->UpdateEvents( socket, entry.events );
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Cancel( int socket )
		{
		outputEpoll_.UnregisterIfExists( socket );
		inputEpoll_.UnregisterIfExists( socket );
		critical_scope_t ics( inputEntryLock_ );
		inputEntries_.Remove( socket );
		critical_scope_t ocs( outputEntryLock_ );
		outputEntries_.Remove( socket );
		critical_scope_t ecs( errorEntryLock_ );		
		errorEntries_.Remove( socket );
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Cancel( int socket, g2::event_type::TYPE type )
		{
		entries_t *entries = NULL;
		Epoll *epoll = NULL;
		critical_scope_t cs;
		
		SetMembersByType( type, entries, epoll, cs );
		
		struct EntryItem &entry = entries->Get( socket );
		entry.events ^= type;
		epoll->UpdateEvents( socket, entry.events );
		}
		
	//-----------------------------------------------------------------------------------------//
	void IODispatch::SetErrorHandler( int socket, handler_t &handler )
		{
		struct EntryItem *entry = NULL;
		critical_scope_t cs( errorEntryLock_ );
		
		if( errorEntries_.HasRegistered( socket ) == true )
			{
			errorEntries_.Get( socket, *entry );
			}
		else
			{
			entry = &( errorEntries_.Add( socket ) );
			}

		entry->handler = handler;
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Dispatch( int timeout )
		{
		bool isStopCalled = false;
		
		while( isStopCalled == false )
			{
			Epoll::event_ptr_t activeEvents = inputEpoll_.Select( timeout );

			for( size_t i = 0; i < activeEvents->GetValidSize(); ++i )
				{
				int socket = activeEvents->GetFDAt( i );
				int events = activeEvents->GetEventsAt( i );

				if( events & event_type::READ &&
					Invoke( socket, inputEntryLock_, inputEntries_ ) == false )
					{
					isStopCalled = true;
					}
				
				if( events & event_type::ERRORS &&
					Invoke( socket, errorEntryLock_, errorEntries_ ) == false )
					{
					isStopCalled = true;
					}
				}
			}
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::Stop()
		{
		stopFlag_.Set( true );
		stopEvent_.Write( 0xff );
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::SetHandler( struct EntryItem &entry, int event, handler_t &handler )
		{
		entry.events |= event;
		entry.handler = handler;
		}

	//-----------------------------------------------------------------------------------------//
	bool IODispatch::StopHandler()
		{
		stopFlag_.Set( false );
		stopEvent_.Read();

		return false;
		}

	//-----------------------------------------------------------------------------------------//
	void IODispatch::SetMembersByType( g2::event_type::TYPE type,
									   entries_t *&entries,
									   g2::Epoll *&epoll,
									   critical_scope_t &cs )
		{
		if( type & event_type::READ )
			{
			entries = &inputEntries_;
			epoll = &inputEpoll_;
			cs.SetLock( inputEntryLock_ );
			}
		else if( type & event_type::WRITE )
			{
			entries = &outputEntries_;
			epoll = &outputEpoll_;
			cs.SetLock( outputEntryLock_ );
			}
		else if( type & event_type::ERRORS )
			{
			entries = &errorEntries_;
			epoll = &inputEpoll_;
			cs.SetLock( errorEntryLock_ );
			}
		else
			{
			assert( "unknown event type was passed" );
			}		
		}

	//-----------------------------------------------------------------------------------------//
	bool IODispatch::Invoke( int socket,
							 g2::MutexLock &lock,
							 entries_t &entries )
		{
		handler_t *handler = NULL;

		{
		critical_scope_t cs( lock );
		if( entries.HasRegistered( socket ) == false )
			{
			return true;
			}

		handler = &( entries.Get( socket ).handler );
		}
		
		return (*handler)();
		}
		
	//-----------------------------------------------------------------------------------------//
	bool IODispatch::OutputEpollHandler()
		{
		Epoll::event_ptr_t activeEvents = outputEpoll_.Select( 1 );
		bool ret = true;
		
		for( size_t i = 0; i < activeEvents->GetValidSize(); ++i )
			{
			int socket = activeEvents->GetFDAt( i );
			int events = activeEvents->GetEventsAt( i );				
			
			if( events & event_type::WRITE &&
				Invoke( socket, outputEntryLock_, outputEntries_ ) == false )
				{
				ret = false;
				}
			
			if( events & event_type::ERRORS &&
				Invoke( socket, errorEntryLock_, errorEntries_ ) == false )
				{
				ret = false;
				}
			}
		
		return ret;
		}

	//-----------------------------------------------------------------------------------------//
	IODispatch::EntryItem::EntryItem()
		:events( 0 ),
		 handler( &( EntryItem::DefaultHandler ) )
		{
		}

	//-----------------------------------------------------------------------------------------//
	bool IODispatch::EntryItem::DefaultHandler()
		{
		throw NetworkException( "EntryItem::DefaultHandler() called" );
		
		return false;
		}
	}


