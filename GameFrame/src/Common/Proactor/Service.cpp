#include "Service.h"
#include "Operation.h"

#define SAFE_DELETE_HANDLE(handle) if (handle != NULL) { ::CloseHandle(handle); handle = NULL; }

Service::Service()
{
    m_Hanlde = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_Hanlde == NULL)
    {
        fprintf(stderr, "Failed create IO completion port. Error code : %d\n", WSAGetLastError());
        return ;
    }
}
Service::~Service()
{
    SAFE_DELETE_HANDLE(m_Hanlde);
}

bool Service::AssociateIoCompletionPort(HANDLE _fileHandle, ULONG_PTR _completionKey, int _threadNum)
{
    if (CreateIoCompletionPort(_fileHandle, m_Hanlde, _completionKey, _threadNum) == NULL)
    {
        fprintf(stderr, "Association socket, error code : %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

void Service::ServiceRun()
{
    DWORD nBytesTransferred = 0;
    LPOVERLAPPED pOverLapped = NULL;
    ULONG_PTR pCompletionKey = NULL;

    while(true)
    {
        ::WSASetLastError(0);
        BOOL ok = ::GetQueuedCompletionStatus(m_Hanlde, &nBytesTransferred, &pCompletionKey, (LPOVERLAPPED*)&pOverLapped, INFINITE);
        DWORD errorCode = ::WSAGetLastError();
        if (!ok && pOverLapped == NULL)
        {
            continue ;
        }
        Operation* pOperation = static_cast<Operation*>(pOverLapped);
        pOperation->DoCompletion(nBytesTransferred, errorCode);
        pOperation->DoDestory(errorCode);
    }
}