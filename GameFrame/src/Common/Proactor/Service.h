#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "NoneCopyable.h"
#include "../Compact.h"

class Service : public NoneCopyable
{
public:
    Service();
    ~Service();
    bool AssociateIoCompletionPort(HANDLE _fileHandle, ULONG_PTR _completionKey = 0, int _threadNum = 0);
    void ServiceRun();
private:
    HANDLE m_Hanlde;
};

#endif