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
     *@brief  ���µ�ǰʱ��
     *@return ���ص�ǰʱ��
     */
    uint Tick(); 
    /*
     *@brief  ������ʱʱ��
     *@return ����ʵ��ʱ��������Ԥ���ʱ����������0�����򷵻�Ԥ������ʵ�ʼ����ֵ
     */
    int Delay();
private:
    uint m_Interval;
    uint m_StartTime;
    uint m_CurrentTime;
};

#endif