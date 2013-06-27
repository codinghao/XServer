#include "TimeService.h"

void TimeService::InitTimeService()
{
#if defined(_WIN32)
    m_StartTime = timeGetTime();
    m_CurrentTime = timeGetTime();
#endif
}
int TimeService::Delay()
{   
    uint oldTime = m_CurrentTime;
#if defined(_WIN32)
    uint curTime = timeGetTime();
#endif
    if (curTime-oldTime < m_Interval)
        return (m_Interval - (curTime-oldTime));
    return 0;
}
uint TimeService::Tick()
{
#if defined(_WIN32)
    m_CurrentTime = timeGetTime();
#endif
    return m_CurrentTime;
}