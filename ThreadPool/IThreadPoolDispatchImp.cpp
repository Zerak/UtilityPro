#include <stdio.h>

#include "IThreadPoolDispatchImp.hpp"

IThreadPoolDispatchImp::IThreadPoolDispatchImp(void)
{
}


IThreadPoolDispatchImp::~IThreadPoolDispatchImp(void)
{
}

void IThreadPoolDispatchImp::processEvents( void *arg )
{
	printf("[IThreadPoolDispatchImp::processEvents] process[%s]\n",(char*)arg);
}