#include "DateTime.h"

DateTime::DateTime()
{
    m_Time = (uint )time( NULL);
}

DateTime::DateTime(const int year, 
             const int month, 
             const int day, 
             const int hour,
             const int minute,
             const int second)
{
    tm t ;
    :: memset(&t , 0, sizeof( tm));
    t.tm_year = year - 1900;
    t.tm_mon = month - 1;
    t.tm_mday = day;

    m_Time = (uint )::mktime(& t);
}

uint DateTime::GetTimeSec() const
{
    return m_Time ;
}

uint DateTime::GetDayBaseSec() const
{
    return (m_Time /(24 * 3600)*(24 * 3600));
}

int DateTime::GetYear() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t->tm_year + 1900);
}

int DateTime::GetMonth() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? (t->tm_mon + 1) : 0;
}

int DateTime::GetDayOfMonth() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_mday : 0;
}

int DateTime::GetDayOfYear() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_yday : 0;
}

int DateTime::GetDayOfWeek() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_wday : 0;
}

int DateTime::GetHour() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_hour : 0;
}

int DateTime::GetMinute() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_min : 0;
}

int DateTime::GetSecond() const
{
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    return (t != NULL) ? t->tm_sec : 0;
}

const tm* DateTime::GetLocalTm (tm* ptm) const
{
    time_t t = GetTimeSec();
#if defined(_WIN32)
    if (::localtime_s(ptm, &t) != 0)
    {
        return NULL;
    }
    return ptm;
#else
    return ::localtime_s(&t);
#endif
}

const tm* DateTime::GetGmtTm(tm* ptm) const
{
    time_t t = GetTimeSec();
#if defined(_WIN32)
    if (::gmtime_s(ptm, &t) != 0)
    {
        return NULL;
    }
    return ptm;
#else
    return ::gmtime_s(&t);
#endif
}

char* DateTime::GetAscTimeStamp(char* buf, int size)
{
 #if defined(_WIN32)
    if (buf == NULL)
    {
        return NULL;
    }
    *buf = '\0';
    tm tmptm;
    const tm * t = GetLocalTm(&tmptm);
    if (t != NULL)
        ::asctime_s(buf, size, t);
    return buf;
#else
    return ::asctime(GetLocalTm());
#endif

}