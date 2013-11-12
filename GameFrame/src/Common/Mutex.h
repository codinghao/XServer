#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "Compact.h"

class Mutex
{
public:
    Mutex();
    ~Mutex();
    void Lock();
    void UnLock();
private:
#if defined(_WIN32)
    CRITICAL_SECTION m_Id;
#endif
};

class AutoLock
{
public:
    AutoLock(Mutex& mtx);
    ~AutoLock();
private:
    Mutex& m_Mutex;
};

#endif