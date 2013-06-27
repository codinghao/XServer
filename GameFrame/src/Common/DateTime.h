#ifndef _TIMER_H_
#define _TIMER_H_

#include "Compact.h"

class DateTime
{
public :
    DateTime();
    DateTime(const uint sectm)
        : m_Time(sectm )
    {}
    DateTime(const int year, 
          const int month, 
          const int day, 
          const int hour,
          const int minute,
          const int second);
    virtual ~DateTime () {}

    uint GetTimeSec () const;
    uint GetDayBaseSec () const;

    int GetYear () const;
    int GetMonth () const;
    int GetDayOfMonth () const;
    int GetDayOfYear () const;
    int GetDayOfWeek () const;
    int GetHour () const;
    int GetMinute () const;
    int GetSecond () const;

    const tm * GetLocalTm(tm* ptm) const;
    const tm * GetGmtTm(tm* ptm) const;

    /*ASCII ∏Ò Ω*/
    char* GetAscTimeStamp(char* buf, int size);

private :
    uint m_Time ;
};

#endif

