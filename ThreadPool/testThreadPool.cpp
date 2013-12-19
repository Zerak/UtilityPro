
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

int main()
{
	LF_ThreadPool *mThreadPool = new LF_ThreadPool(2);
	IThreadPoolDispatchImp *pDispatch = new IThreadPoolDispatchImp();

	mThreadPool->dispatch( pDispatch,"abcde" );
	pause();
	mThreadPool->dispatch( pDispatch,"abcdef" );
	pause();

	delete mThreadPool;
	delete pDispatch;
	return 0;
}