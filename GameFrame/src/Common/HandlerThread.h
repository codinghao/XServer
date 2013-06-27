#ifndef _HANDLER_THREAD_H_
#define _HANDLER_THREAD_H_

#include "Thread.h"

class NetworkServiceImpl;

class HandlerThread : public Thread
{
public:
    HandlerThread(NetworkServiceImpl* pImpl);
    virtual ~HandlerThread();

    void Run();

private:
    NetworkServiceImpl* m_pServiceImpl;
};

#endif