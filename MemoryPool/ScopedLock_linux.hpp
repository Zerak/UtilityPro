

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/17/2013 10:58
 *     File_base        : ScopedLock_linux
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _ScopedLock_linux_hpp_
#define _ScopedLock_linux_hpp_

#include <pthread.h>

class ScopedLockImp
{
public:
	ScopedLockImp()
	{
		pthread_mutex_init( &mt, 0 );
	}
	~ScopedLockImp()
	{
		pthread_mutex_destroy( &mt );
	}

	void lock()
	{
		pthread_mutex_lock( &mt );
	}

	void unlock()
	{
		pthread_mutex_unlock( &mt );
	}

private:
	pthread_mutex_t mt;
};

#endif