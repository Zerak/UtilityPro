

// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 12/17/2013 16:16
 *     File_base        : LF_ThreadPool
 *     File_ext         : hpp
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : leader follower thread pool
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _LF_ThreadPool_hpp_
#define _LF_ThreadPool_hpp_

#define MaxThreadPoolCount 8

#include "SysWrapper.hpp"

class IThreadPoolDispatch;
class ArgThread;

class LF_ThreadPool
{
public:
	LF_ThreadPool( int maxThreads = MaxThreadPoolCount );
	~LF_ThreadPool(void);

public:
	int dispatch( IThreadPoolDispatch *pDispatch, void *args );

private:
	static thread_result_t THREAD_CALL wrapperFunc( void *args );
	int saveThread( ArgThread *thread );

private:
	int		m_nMaxThreads;
	int		m_nIndex;
	int		m_nTotal;	// current thread count
	bool	m_bShutdown;
	
	pthread_mutex_t	m_MainMutex;
	pthread_cond_t	m_IdleCond;
	pthread_cond_t	m_FullCond;
	pthread_cond_t	m_EmptyCond;

	ArgThread	**m_pThreadList;
};


#endif