
//#define Plateform_Win32
#define Plateform_Linux

#ifndef nullptr
#define nullptr 0
#endif

#include "GNUMemoryPool.hpp"
#include <assert.h>
#include <iostream>
using namespace std;

#define Max_ThreadCount 2

typedef struct
{
    int header;
    int cmd;
    char contents[2000];
} ArgEvent;

typedef GNUMemoryPool<ArgEvent> ArgEventPool;

#ifdef Plateform_Win32

DWORD WINAPI threadFun (LPVOID lpParam)
{
    ArgEventPool *eventPool = ( ArgEventPool* ) lpParam;

    for (int i = 0; i < 0.5 * 10000; i++)
    {
        ArgEvent *event = eventPool->get();
        cout << "[" << GetCurrentThreadId() << "]" << "##" << endl;
        assert(event != nullptr);
        event->header = i;
        event->cmd = i;
        memcpy(event->contents, "test", 2000);

        Sleep(10);
        eventPool->set(event);
        cout << "[" << GetCurrentThreadId() << "]" << "*" << endl;
    }

    return 0;
}

int main ()
{
    //|||||||||||||||||||||||||||
    // test GNUMemoryPool
    //|||||||||||||||||||||||||||

    ArgEventPool eventPool;
    eventPool.init(2 * 1);

    DWORD threadId[Max_ThreadCount];
    HANDLE threadHandle[Max_ThreadCount];

    for (int i = 0; i < Max_ThreadCount; i++)
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

#else Plateform_Linux

#include <pthread.h>
#include <unistd.h>

void *threadFun (void *param)
{
    ArgEventPool *eventPool = ( ArgEventPool* ) param;
    for (int i = 0; i < 0.5 * 10000; i++)
    {
        ArgEvent *event = eventPool->get();
        cout << "[" << pthread_self() << "]" << "##" << endl;
        assert(event != nullptr);
        event->header = i;
        event->cmd = i;
        memcpy(event->contents, "test", 2000);

        usleep(10 * 1000);
        eventPool->set(event);
        cout << "[" << pthread_self() << "]" << "*" << endl;
    }
    return nullptr;
}

int main ()
{
    //|||||||||||||||||||||||||||
    // test GNUMemoryPool
    //|||||||||||||||||||||||||||

    ArgEventPool eventPool;
    eventPool.init(2 * 1);

    pthread_t threadHandle[Max_ThreadCount];
    for (int i = 0; i < Max_ThreadCount; i++)
    {
        int nRet = pthread_create(
                &threadHandle[i],
                nullptr,
                threadFun,
                (void*)&eventPool
                );
        if( nRet != 0 )
        {
            cout<<"create thread err[%d]"<<nRet<<endl;
        }
        cout<<"create thread["<<threadHandle[i]<<"]"<<endl;
    }
    pause();
    return 0;
}
#endif
