#ifndef _SERVICE_H_
#define _SERVICE_H_

#define SAFE_DELETE_HANDLE(handle) if (handle != NULL) { ::CloseHandle(handle); handle = NULL; }

class IoCompletionPortHandle
{
public:
    IoCompletionPortHandle()
    {
        m_Hanlde = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
        if (m_Hanlde == NULL)
        {
            fprintf(stderr, "Failed create IO completion port. Error code : %d\n", WSAGetLastError());
            return NULL;
        }
    }

    ~IoCompletionPortHandle()
    {
        SAFE_DELETE_HANDLE(m_Hanlde);
    }

    bool AssociateIoCompletionPort(HANDLE _fileHandle, ULONG_PTR _completionKey = NULL, int _threadNum = 0)
    {
        if (CreateIoCompletionPort(_fileHandle, m_Hanlde, _completionKey, _threadNum) == NULL)
        {
            fprintf(stderr, "Association socket, error code : %d\n", WSAGetLastError());
            return false;
        }

        return true;
    }

    HANDLE GetHandle() {  return m_Hanlde;  }

private:
    HANDLE m_Hanlde;
};


class WsaIoctl
{
public:
    

private:
    LPFN_ACCEPTEX m_AcceptEx;
    LPFN_GETACCEPTEXSOCKADDRS m_GetAcceptExSockAddrs;
};

class Service : public NoneCopyable
{
public:
    Service()
    {}
    ~Service()
    {}

    void ServiceRun()
    {
        DWORD nBytesTransferred = 0;
        LPOVERLAPPED pOverLapped = NULL;
        PULONG_PTR pCompletionKey = NULL;

        while(true)
        {
            ::WSASetLastError(0);

            bool ok = ::GetQueuedCompletionStatus(m_ServiceHandle.GetHandle(), pCompletionKey, &pOverLapped, INFINITE);
            if (!ok && pOverLapped == NULL)
            {
                continue ;
            }
            
            DWORD errorCode = ::WSAGetLastError();

            Operation* pOperation = static_cast<Operation*>(pOverLapped);
            pOperation->DoInvoke(nBytesTransferred, errorCode);
        }
    }

private:
    IoCompletionPortHandle m_ServiceHandle;
};

#endif