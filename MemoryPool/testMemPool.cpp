
#define Plateform_Win32
//#define Plateform_Linux

#include "GNUMemoryPool.hpp"
#include <assert.h>
#include <iostream>
using namespace std;

#define Max_ThreadCount 2

typedef struct
{
	int		header;
	int		cmd;
	char	contents[2000];
}ArgEvent;

typedef GNUMemoryPool<ArgEvent> ArgEventPool;

#ifdef Plateform_Win32
DWORD WINAPI threadFun( LPVOID lpParam )
{
	ArgEventPool *eventPool = (ArgEventPool*)lpParam;

	for ( int i = 0; i < 0.5*10000; i ++ )
	{
		ArgEvent *event = eventPool->get();
		cout<<"["<<GetCurrentThreadId()<<"]"<<"."<<endl;
		assert( event!= nullptr );
		event->header = i;
		event->cmd = i;
		memcpy( event->contents, "test", 2000 );

		eventPool->set( event );
		cout<<"["<<GetCurrentThreadId()<<"]"<<"¡£"<<endl;
	}

	return 0;
}

int main()
{
	//|||||||||||||||||||||||||||
	// test GNUMemoryPool
	//|||||||||||||||||||||||||||

	int size = sizeof(ArgEvent);
	ArgEventPool eventPool;
	eventPool.init( 2 * 1 );

	DWORD threadId[Max_ThreadCount];
	HANDLE threadHandle[Max_ThreadCount];

	for ( int i = 0; i < 2; i ++ )
	{
		threadHandle[i] = CreateThread(
			nullptr,
			0,
			threadFun,
			&eventPool,
			0,
			&threadId[i]
			);
	}
	system("pause");
	return 0;
}

#elif Plateform_Linux
int main()
{
	return 0;
}
#endif
