

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/19/2013 11:52
 *     File_base        : IThreadPoolDispatchImp
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : 
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _IThreadPoolDispatchImp_hpp_
#define _IThreadPoolDispatchImp_hpp_

#include "IThreadPoolDispatch.hpp"

class EventHandleManager;

class IThreadPoolDispatchImp : public IThreadPoolDispatch
{
public:
	IThreadPoolDispatchImp(void);
	~IThreadPoolDispatchImp(void);

public:
	virtual void processEvents( void *arg );

private:
	EventHandleManager	*m_pHandleMgr;
};



#endif