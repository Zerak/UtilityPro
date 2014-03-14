
#include <iostream>
using namespace std;

#include "LF_ThreadPool.hpp"
#include "IThreadPoolDispatchImp.hpp"

void pause()
{
#ifdef PlatformWin32
	system("pause");
#else
	pause();
#endif // PlatformWin32
}
                                 
void sleep( int microseconds )
{
#ifdef PlatformWin32
	Sleep( microseconds * 1000 );
#else
	sleep( microseconds );
#endif // PlatformWin32
}

int main()
{
	LF_ThreadPool *mThreadPool = new LF_ThreadPool(2);
	IThreadPoolDispatchImp *pDispatch = new IThreadPoolDispatchImp(); 

	printf("Q or q for quit\nN or n for process\n");

	char buf[128];
	memset( buf, 0, 128 );
	int c;
	while ( true )
	{
		c = getchar();
		switch ( c )
		{
		case 'n':
			mThreadPool->dispatch( pDispatch,"abcde" );
			break;

		case 'q':
		case 'Q':
			exit(0);
			break;

		case '\n':
			break;

		default:
			system("color 04");
			system("cls");
			printf("Q or q for quit\nN or n for process\n");
			break;
		}
	}

	delete mThreadPool;
	delete pDispatch;
	return 0;
}