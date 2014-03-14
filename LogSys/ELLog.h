
// -*- C++ -*-
//==========================================================
/**
 *     Created_datetime : 2/21/2013 17:14
 *     File_base        : LogSys
 *     File_ext         : h
 *     Author           : GNUnix <Kingbug2010@gmail.com>
 *     Description      : log
 *
 *     <Change_list>
 */
//==========================================================
#ifndef _ELLog_hpp
#define _ELLog_hpp

#include <fstream>
#include <string>
#include <sstream>
#include <ctime>
#include <stdarg.h>
#include "CyLock.h"
#include "Singleton.h"
using namespace std;

class LogSys : public CyLock
{
public:
    LogSys();
    ~LogSys();

    bool CommonLogInit(const char *name); //open default log file
    void LogOutLn(const char *value, ...)
    {
        lock();
        char buffer[2048];
        memset( buffer, 0, 2048 ) ;

        va_list argptr;

        va_start(argptr, value);
		vsprintf_s( buffer, value,argptr );
        va_end(argptr);

        string time = "[" + GetTimeStr() + "]" + buffer;
		sprintf_s( buffer, "%s", time.c_str() );

        if (m_bEnabled)
        {
            m_tOLogFile << buffer << endl;
        }
        unlock();
    }

private:
    bool Open(string sFileName);
    void Close();
    void Enable();
    void Disable();
    string GetTimeStr();

    template <typename T> LogSys& operator<<(const T& value)
    {
        if (m_bEnabled)
        {
            //m_tOLogFile<<GetTimeStr()<<value<<endl;
            m_tOLogFile << value;
        }
        return (*this);
    }

private:
    ofstream m_tOLogFile;
    bool m_bEnabled;
} ;

typedef Singleton<LogSys > AppLogSys;

#endif