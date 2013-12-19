#include "SysWrapper.hpp"


int pthread_mutex_init( pthread_mutex_t *mutex, void *arg )
{
	*mutex = CreateMutex( nullptr, false, nullptr );

	return *mutex == nullptr ? GetLastError() : 0;
}

int pthread_mutex_destroy( pthread_mutex_t *mutex )
{
	int ret = CloseHandle( *mutex );
	return ret == 0 ? GetLastError() : 0;
}

int pthread_mutex_lock( pthread_mutex_t *mutex )
{
	int ret = WaitForSingleObject( *mutex, INFINITE );
	return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
}

int pthread_mutex_unlock( pthread_mutex_t *mutex )
{
	int ret = ReleaseMutex( *mutex );
	return ret == 0 ? GetLastError() : 0;
}

int pthread_cond_init( pthread_cond_t *cond, void *arg )
{
	*cond = CreateEvent( nullptr, false, false, nullptr );
	return *cond == nullptr ? GetLastError() : 0;
}

int pthread_cond_destroy( pthread_cond_t *cond )
{
	int ret = CloseHandle( *cond );
	return ret == 0 ? GetLastError() : 0;
}

int pthread_cond_wait( pthread_cond_t *cond, pthread_mutex_t *mutex )
{
	int ret = 0;
	pthread_mutex_unlock( mutex );
	ret = WaitForSingleObject( *cond, INFINITE );
	pthread_mutex_lock( mutex );

	return WAIT_OBJECT_0 == ret ? 0 : GetLastError();
}

int pthread_cond_signal( pthread_cond_t *cond )
{
	int ret = SetEvent( *cond );
	return ret == 0 ? GetLastError() : 0;
}

int pthread_attr_init( pthread_attr_t *attr )
{
	memset( attr, 0, sizeof(pthread_attr_t) );
	return 0;
}

int pthread_attr_destroy( pthread_attr_t *attr )
{
	return 0;
}

int pthread_attr_setstacksize( pthread_attr_t *attr, int stacksize )
{
	attr->stacksize = stacksize;
	return 0;
}

int pthread_attr_setdetachstate( pthread_attr_t *attr, int detachstate )
{
	attr->detachstate = detachstate;
	return 0;
}

int pthread_create( pthread_t *thread, const pthread_attr_t *attr, unsigned int( __stdcall *start_routine ) (void*), void *arg )
{
	HANDLE handle;
	if ( attr == nullptr )
	{
		handle = (HANDLE)_beginthreadex( nullptr, 0, start_routine, arg, 0, thread );
	}
	else
	{
		handle = (HANDLE)_beginthreadex( nullptr, attr->stacksize, start_routine, arg, 0, thread );
	}

	return handle > 0 ? 0 : GetLastError();	// succeed return 0
}

pthread_t pthread_self( void )
{
	return GetCurrentThreadId();
}