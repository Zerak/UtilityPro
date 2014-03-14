
// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 3/14/2014 15:04
 *     File_base        : CyLock
 *     File_ext         : CyLock
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _CyLock_hpp_
#define _CyLock_hpp_

#include <windows.h>

class CyLock
{
public:
	CyLock();
	~CyLock();

	void lock();
	void unlock();

private:
	CRITICAL_SECTION cs;
};

#endif
