#ifndef _TIME_SERVICE_H_
#define _TIME_SERVICE_H_

#include "Compact.h"

class TimeService
{
public:
    TimeService(const int interval = 0)
        : m_Interval(interval)
        , m_StartTime(0)
        , m_CurrentTime(0)
    {}
    ~TimeService(){};

    void InitTimeService();
    uint CurrentTime() const { return m_CurrentTime; }
    uint StarUpTime() const { return m_StartTime; }
    uint RunTime() const { return (m_CurrentTime-m_StartTime); }
    /*
     *@brief  更新当前时间
     *@return 返回当前时间
     */
    uint Tick(); 
    /*
     *@brief  计算延时时间
     *@return 跳动实际时间间隔大于预设的时间间隔，返回0，否则返回预设间隔与实际间隔差值
     */
    int Delay();
private:
    uint m_Interval;
    uint m_StartTime;
    uint m_CurrentTime;
};

#endif