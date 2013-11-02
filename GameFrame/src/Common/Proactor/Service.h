#ifndef _PROACTOR_H_
#define _PROACTOR_H_

#define SAFE_DELETE_HANDLE(handle) if (handle != NULL) { CloseHandle(handle); handle = NULL; }

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
        if (CreateIoCompletionPort(_fileHandle, m_Hanlde, _completionKey, 0) == NULL)
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

private:
    IoCompletionPortHandle m_ServiceHandle;
};

#endif