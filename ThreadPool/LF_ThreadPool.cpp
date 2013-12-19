#include <string.h>

#include "LF_ThreadPool.hpp"
#include "IThreadPoolDispatch.hpp"

class ArgThread
{
public:
	pthread_t mId;
	pthread_mutex_t mMutex;
	pthread_cond_t	mCond;
	IThreadPoolDispatch	*mDis;
	void	*mArg;
	LF_ThreadPool *mParent;
};

LF_ThreadPool::LF_ThreadPool( int maxThreads /*= MaxThreadPoolCount*/ )
{
	if ( maxThreads <= 0 )
	{
		maxThreads = MaxThreadPoolCount;
	}

	m_nMaxThreads = maxThreads;
	m_nIndex = 0;
	m_nTotal = 0;
	m_bShutdown = false;

	pthread_mutex_init( &m_MainMutex, nullptr );
	pthread_cond_init( &m_IdleCond, nullptr );
	pthread_cond_init( &m_FullCond, nullptr );
	pthread_cond_init( &m_EmptyCond, nullptr );

	m_pThreadList = new ArgThread*[m_nMaxThreads];
	memset( m_pThreadList, 0, sizeof(void*) * m_nMaxThreads );
}

LF_ThreadPool::~LF_ThreadPool(void)
{
	pthread_mutex_lock( &m_MainMutex );

	if ( m_nIndex < m_nTotal )
	{
		pthread_cond_wait( &m_FullCond, &m_MainMutex );
	}

	m_bShutdown = true;

	for( int i = 0; i < m_nIndex; i ++ )
	{
		ArgThread *pThread = m_pThreadList[i];
		if ( pThread == nullptr )
			continue;
		pthread_mutex_lock( &pThread->mMutex );
		pthread_cond_signal( &pThread->mCond );
		pthread_mutex_unlock( &pThread->mMutex );
	}

	if ( m_nTotal > 0 )
	{
		pthread_cond_wait( &m_EmptyCond, &m_MainMutex );
	}

	for ( int i = 0; i < m_nIndex; i ++ )
	{
		ArgThread *pThread = m_pThreadList[i];
		if ( pThread == nullptr )
			continue;
		pthread_mutex_destroy( &pThread->mMutex );
		pthread_cond_destroy( &pThread->mCond );
		delete pThread;
		pThread = nullptr;
	}

	pthread_mutex_unlock( &m_MainMutex );

	m_nIndex = 0;

	pthread_mutex_destroy( &m_MainMutex );
	pthread_cond_destroy( &m_IdleCond );
	pthread_cond_destroy( &m_FullCond );
	pthread_cond_destroy( &m_EmptyCond );

	if ( m_pThreadList != nullptr )
	{
		delete []m_pThreadList;
		m_pThreadList = nullptr;
	}
}

int LF_ThreadPool::dispatch( IThreadPoolDispatch *pDispatch, void *args )
{
	int ret = 0;

	pthread_mutex_lock( &m_MainMutex );

	if ( m_nIndex <= 0 && m_nTotal >= m_nMaxThreads )
	{
		pthread_cond_wait( &m_IdleCond, &m_MainMutex );
	}

	if ( m_nIndex <= 0 )
	{
		ArgThread *pThread = new ArgThread();
		if ( pThread == nullptr )
		{
			ret = -1;
			pthread_mutex_unlock( &m_MainMutex );
			return ret;
		}
		pthread_mutex_init( &pThread->mMutex, nullptr );
		pthread_cond_init( &pThread->mCond, nullptr );
		pThread->mDis = pDispatch;
		pThread->mArg = args;
		pThread->mParent = this;

		pthread_attr_t attr;
		pthread_attr_init( &attr );
		pthread_attr_setdetachstate( &attr, PTHREAD_CREATE_DETACHED );
		if ( 0 == pthread_create( &pThread->mId, &attr, wrapperFunc, pThread ) )
		{
			m_nTotal ++;
		}
		else
		{
			ret = -1;
			pthread_mutex_destroy( &pThread->mMutex );
			pthread_cond_destroy( &pThread->mCond );
			delete pThread;
			pThread = nullptr;
		}
		pthread_attr_destroy( &attr );
	}
	else
	{
		m_nIndex --;
		ArgThread *pThread = m_pThreadList[m_nIndex];
		m_pThreadList[m_nIndex] = nullptr;
		if ( pThread )
		{
			pThread->mDis = pDispatch;
			pThread->mArg = args;
			pThread->mParent = this;

			pthread_mutex_lock( & pThread->mMutex );
			pthread_cond_signal( & pThread->mCond );
			pthread_mutex_unlock( & pThread->mMutex );
		}
	}

	pthread_mutex_unlock( &m_MainMutex );

	return ret;
}

thread_result_t THREAD_CALL LF_ThreadPool::wrapperFunc( void *args )
{
	ArgThread *pThread = (ArgThread*)args;
	while ( pThread->mParent->m_bShutdown == false )
	{
		pThread->mDis->processEvents( pThread->mArg );

		if ( pThread->mParent->m_bShutdown )
			break;

		pthread_mutex_lock( &pThread->mMutex );
		if ( 0 == pThread->mParent->saveThread( pThread ) )
		{
			pthread_cond_wait( &pThread->mCond, &pThread->mMutex );
			pthread_mutex_unlock( &pThread->mMutex );
		}
		else
		{
			pthread_mutex_unlock( &pThread->mMutex );
			pthread_cond_destroy( &pThread->mCond );
			pthread_mutex_destroy( &pThread->mMutex );

			delete pThread;
			pThread = nullptr;
			break;
		}
	}

	if ( pThread == nullptr )
	{
		pthread_mutex_lock( &pThread->mParent->m_MainMutex );
		pThread->mParent->m_nTotal--;
		if ( pThread->mParent->m_nTotal <= 0 )
		{
			pthread_cond_signal( &pThread->mParent->m_EmptyCond );
		}
		pthread_mutex_unlock( &pThread->mParent->m_MainMutex );
	}
	return 0;
}

int LF_ThreadPool::saveThread( ArgThread *thread )
{
	int ret = -1;

	pthread_mutex_lock( &m_MainMutex );
	if ( m_nIndex < m_nMaxThreads )
	{
		m_pThreadList[m_nIndex] = thread;
		m_nIndex ++;
		ret = 0;

		pthread_cond_signal( &m_IdleCond );

		if ( m_nIndex >= m_nTotal )
		{
			pthread_cond_signal( &m_FullCond );
		}
	}
	pthread_mutex_unlock( &m_MainMutex );

	return ret;
}