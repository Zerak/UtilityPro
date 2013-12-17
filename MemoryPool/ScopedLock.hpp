

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/13/2013 16:55
 *     File_base        : ScopedLock
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _ScopedLock_hpp_
#define _ScopedLock_hpp_

#ifdef __linux__ || __linux
#include "ScopedLock_linux.hpp"
#elif _WIN32
#include "ScopedLock_win32.hpp"
#endif

class ScopedLock
{
public:
	ScopedLock(){}
	~ScopedLock(){}

	void lock()
	{
		locker.lock();
	}

	void unlock()
	{
		locker.unlock();
	}

private:
	ScopedLockImp locker;
};

#endif