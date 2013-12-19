

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/19/2013 11:53
 *     File_base        : IThreadPoolDispatch
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _IThreadPoolDispatch_hpp_
#define _IThreadPoolDispatch_hpp_

class IThreadPoolDispatch
{
public:
	IThreadPoolDispatch(void){}
	virtual ~IThreadPoolDispatch(){}

public:
	virtual void processEvents( void *arg ) = 0;
};

#endif