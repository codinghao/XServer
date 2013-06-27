#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include "ObjectPool.h"
#include "TimeService.h"
#include "Utils.h"

void ObjectPoolTest()
{
    TimeService t2;
    t2.InitTimeService();
    for (int i = 0; i < 1000000; i ++)
    {
        TimeService* pObj = new TimeService();
        delete pObj;
    }
    t2.Tick();
    std::cout << "New 1 time : " << t2.RunTime() << std::endl;

    TimeService t1;
    t1.InitTimeService();
    for (int i = 0; i < 1000000; i ++)
    {
        TimeService* pObj = ObjectPool<TimeService>::Instance().Pop();
        ObjectPool<TimeService>::Instance().Push(pObj);
    }
    t1.Tick();
    std::cout << "Pool 1 time : " << t1.RunTime() << std::endl;

    TimeService t3;
    t3.InitTimeService();
    for (int i = 0; i < 1000000; i ++)
    {
        TimeService* pObj = ObjectPool<TimeService>::Instance().Pop();
        ObjectPool<TimeService>::Instance().Push(pObj);
    }
    t3.Tick();
    std::cout << "Pool 2 time : " << t3.RunTime() << std::endl;

    TimeService t4;
    t4.InitTimeService();
    for (int i = 0; i < 1000000; i ++)
    {
        TimeService* pObj = new TimeService();
        delete pObj;
    }
    t4.Tick();
    std::cout << "New 2 time : " << t4.RunTime() << std::endl;
}

void MemAllocTest()
{
    int nSize = 0;

    TimeService t1;
    t1.InitTimeService();
    for (int i = 0; i < 10000000; i ++)
    {
        nSize = RandLimits(4, 512);
        void* p = MemAllocT.Alloc(nSize);
        MemAllocT.Dealloc(p, nSize);
    }
    t1.Tick();
    std::cout << "Pool 1 time : " << t1.RunTime() << std::endl;

    TimeService t2;
    t2.InitTimeService();
    for (int i = 0; i < 10000000; i ++)
    {
        nSize = RandLimits(4, 512);
        char* p = new char[nSize];
        delete [] p;
    }
    t2.Tick();
    std::cout << "New 1 time : " << t2.RunTime() << std::endl;

    TimeService t3;
    t3.InitTimeService();
    for (int i = 0; i < 10000000; i ++)
    {
        nSize = RandLimits(4, 512);
        void* p = MemAllocT.Alloc(nSize);
        MemAllocT.Dealloc(p, nSize);
    }
    t3.Tick();
    std::cout << "Pool 2 time : " << t3.RunTime() << std::endl;

    TimeService t4;
    t4.InitTimeService();
    for (int i = 0; i < 10000000; i ++)
    {
        nSize = RandLimits(4, 512);
        char* p = new char[nSize];
        delete [] p;
    }
    t4.Tick();
    std::cout << "New 2 time : " << t4.RunTime() << std::endl;
}

#endif