#include "readwritelock.h"
#include "exception.h"
#include <errno.h>

namespace g2
	{
	//-----------------------------------------------------------------------------------------//
	ReadWriteLock::ReadWriteLock()
		{
		pthread_rwlock_init( &lock_, NULL );
		}

	//-----------------------------------------------------------------------------------------//
	ReadWriteLock::~ReadWriteLock()
		{
		pthread_rwlock_destroy( &lock_ );
		}

	//-----------------------------------------------------------------------------------------//
	void ReadWriteLock::AcquireReadLock()
		{
		int result = pthread_rwlock_rdlock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "ReadWriteLock::AcquireReadLock() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool ReadWriteLock::TryAcquireReadLock()
		{
		int result = pthread_rwlock_tryrdlock( &lock_ );
		if( result == 0 )
			{
			return true;
			}
		else if( result == EBUSY )
			{
			return false;
			}

		throw Exception( "ReadWriteLock::TryAcquireReadLock() failed", result );
		}
	
	//-----------------------------------------------------------------------------------------//
	void ReadWriteLock::AcquireWriteLock()
		{
		int result = pthread_rwlock_wrlock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "ReadWriteLock::AcquireWriteLock() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	bool ReadWriteLock::TryAcquireWriteLock()
		{
		int result = pthread_rwlock_trywrlock( &lock_ );
		if( result == 0 )
			{
			return true;
			}
		else if( result == EBUSY )
			{
			return false;
			}

		throw Exception( "ReadWriteLock::TryAcquireWriteLock() failed", result );
		}

	//-----------------------------------------------------------------------------------------//
	void ReadWriteLock::Release()
		{
		int result = pthread_rwlock_unlock( &lock_ );
		if( result != 0 )
			{
			throw Exception( "ReadWriteLock::Release() failed", result );
			}
		}

	//-----------------------------------------------------------------------------------------//
	ReadLock::ReadLock( ReadWriteLock &rwlock )
		:rwlock_( rwlock )
		{
		}

	//-----------------------------------------------------------------------------------------//
	void ReadLock::Acquire()
		{
		rwlock_.AcquireReadLock();
		}

	//-----------------------------------------------------------------------------------------//
	void ReadLock::Release()
		{
		rwlock_.Release();
		}

	//-----------------------------------------------------------------------------------------//
	bool ReadLock::TryAcquire()
		{
		return rwlock_.TryAcquireReadLock();
		}

	//-----------------------------------------------------------------------------------------//
	WriteLock::WriteLock( ReadWriteLock &rwlock )
		:rwlock_( rwlock )
		{
		}
	
	//-----------------------------------------------------------------------------------------//
	void WriteLock::Acquire()
		{
		rwlock_.AcquireWriteLock();
		}

	//-----------------------------------------------------------------------------------------//
	void WriteLock::Release()
		{
		rwlock_.Release();
		}

	//-----------------------------------------------------------------------------------------//
	bool WriteLock::TryAcquire()
		{
		return rwlock_.TryAcquireWriteLock();
		}
	}

