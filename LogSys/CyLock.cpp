#include "CyLock.h"

CyLock::CyLock()
{
	InitializeCriticalSection( &cs );
}

CyLock::~CyLock()
{
	DeleteCriticalSection( &cs );
}

void CyLock::lock()
{
	EnterCriticalSection( &cs );
}

void CyLock::unlock()
{
	LeaveCriticalSection( &cs );
}