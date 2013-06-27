#ifndef _MAIN_SERVICE_H_
#define _MAIN_SERVICE_H_

#include "Thread.h"
#include "TimeService.h"

class MainService : public Thread
{
public:
    MainService() 
        : m_TimeService(1000)
    {}
    ~MainService(){}
    void Init() const;
    void Run();
private:
    TimeService m_TimeService;
};

#endif