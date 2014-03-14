#include "ELLog.h"

ELLog::ELLog()
: m_bEnabled(true)
{
}

ELLog::~ELLog()
{
}

bool ELLog::Open(string sFileName)
{
    m_tOLogFile.open(sFileName.c_str(), ios_base::out | ios_base::trunc);
    if ( !m_tOLogFile )
    {
        return false;
    }
    return true;
}

void ELLog::Close()
{
    if (m_tOLogFile.is_open())
    {
        m_tOLogFile.close();
    }
}

bool ELLog::CommonLogInit(const char *name)
{
    time_t tNowTime;
	time(&tNowTime);
	tm *tLocalTime = nullptr;
	int err = localtime_s( tLocalTime, &tNowTime );
	if ( err != 0 )
	{
		printf("[ELLog::CommonLogInit] errno[%d]\n",errno);
		return false;
	}

    char str[128];
    sprintf_s(str, "%s_%04d-%02d-%02d-%02d%02d%02d.log", name, tLocalTime->tm_year + 1900,
            tLocalTime->tm_mon + 1,
            tLocalTime->tm_mday,
            tLocalTime->tm_hour,
            tLocalTime->tm_min,
            tLocalTime->tm_sec);

    return Open(str);
}

void ELLog::Enable()
{
    m_bEnabled = true;
}

void ELLog::Disable()
{
    m_bEnabled = false;
}

string ELLog::GetTimeStr()
{
    time_t tNowTime;
	time(&tNowTime);
	tm *tLocalTime = nullptr;
	int err = localtime_s( tLocalTime, &tNowTime );
	if ( err != 0 )
	{
		printf("[ELLog::GetTimeStr] errno[%d]\n",errno);
		return "";
	}
    char str[128];
    sprintf_s(str, "%04d-%02d-%02d %02d:%02d:%02d", tLocalTime->tm_year + 1900,
            tLocalTime->tm_mon + 1,
            tLocalTime->tm_mday,
            tLocalTime->tm_hour,
            tLocalTime->tm_min,
            tLocalTime->tm_sec);
    return str;
}