#ifndef _XSERVER_THREAD_H_
#define _XSERVER_THREAD_H_

#include "Compact.h"

class Thread
{
public:
    enum ThreadStatus
    {
        READY,  
        RUNNING,
        EXIT
    };

public:
    Thread();
    virtual ~Thread();
    void Start();
    void Join();
    virtual void Run() = 0;
    void Stop();
    void Exit();
    bool IsShutDown();
    ThreadStatus GetStatus();
    void SetStatus(ThreadStatus status);
private:
#if defined(_WIN32)
    HANDLE m_Handle;
#endif
    uint m_Id;
    ThreadStatus  m_Status;
    volatile long m_ShutDown;
};

#endif