

// -*- C++ -*-
//==========================================================
/**
*     Created_datetime : 12/19/2013 9:13
*     File_base        : SysWrapper
*     File_ext         : hpp
*     Author           : GNUnix <Kingbug2010@gmail.com>
*     Description      : 
*
*     <Change_list>
*/
//==========================================================
#ifndef _SysWrapper_hpp_
#define _SysWrapper_hpp_

#ifdef PlatformWin32

#include <atlbase.h>
#include <process.h>

#define PTHREAD_CREATE_DETACHED 1
#define THREAD_CALL __stdcall

struct pthread_attr_t
{
	int stacksize;
	int detachstate;
};
typedef HANDLE pthread_mutex_t;
typedef HANDLE pthread_cond_t;
typedef unsigned int pthread_t;
typedef unsigned int thread_result_t;

/////////////////////////////////////////
//NODE: All Function Return 0 Must Be OK
/////////////////////////////////////////
// pthread_mutex
int pthread_mutex_init( pthread_mutex_t *mutex, void *arg );
int pthread_mutex_destroy( pthread_mutex_t *mutex );
int pthread_mutex_lock( pthread_mutex_t *mutex );
int pthread_mutex_unlock( pthread_mutex_t *mutex );

// pthread_cond
int pthread_cond_init( pthread_cond_t *cond, void *arg );
int pthread_cond_destroy( pthread_cond_t *cond );
int pthread_cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex );
int pthread_cond_signal( pthread_cond_t *cond );

// pthread_attr
int pthread_attr_init( pthread_attr_t *attr );
int pthread_attr_destroy( pthread_attr_t *attr );
int pthread_attr_setstacksize( pthread_attr_t *attr, int stacksize );
int pthread_attr_setdetachstate( pthread_attr_t *attr, int detachstate );

// pthread_create
int pthread_create( pthread_t *thread, const pthread_attr_t *attr, 
				   thread_result_t( THREAD_CALL *start_routine ) (void*), void *arg );
pthread_t pthread_self( void );

#else
////////////////////////////////////////////////
//PlatformLinux
#include <pthread.h>

typedef void * thread_result_t;
#define THREAD_CALL

#endif

#endif // _SysWrapper_hpp_