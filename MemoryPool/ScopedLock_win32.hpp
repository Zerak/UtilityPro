

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/17/2013 11:01
 *     File_base        : ScopedLock_win32
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _ScopedLock_win32_hpp_
#define _ScopedLock_win32_hpp_

#include <windows.h>

class ScopedLockImp
{
public:
	ScopedLockImp()
	{
		InitializeCriticalSection( &csLock );
	}
	~ScopedLockImp()
	{
		DeleteCriticalSection( &csLock );
	}

	void lock()
	{
		EnterCriticalSection( &csLock );
	}

	void unlock()
	{
		LeaveCriticalSection( &csLock );
	}

private:
	CRITICAL_SECTION csLock;
};

#endif