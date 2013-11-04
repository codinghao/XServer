#ifndef _IOCTL_FUNC_SET_H_
#define _IOCTL_FUNC_SET_H_

class IoctlFuncSet
{
public:
    static BOOL AcceptEx(SOCKET sListenSocket, SOCKET sAcceptSocket, PVOID lpOutputBuffer, DWORD dwReceiveDataLength, 
        DWORD dwLocalAddressLength, DWORD dwRemoteAddressLength, LPDWORD lpdwBytesReceived, LPOVERLAPPED lpOverlapped)
    {
        return (*m_AcceptEx)(sListenSocket, sAcceptSocket, lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, 
            dwRemoteAddressLength, lpdwBytesReceived, lpOverlapped);
    }

    static BOOL GetAcceptExSockaddrs(PVOID lpOutputBuffer, DWORD dwReceiveDataLength, DWORD dwLocalAddressLength, 
        DWORD dwRemoteAddressLength, struct sockaddr **LocalSockaddr, LPINT LocalSockaddrLength, struct sockaddr **RemoteSockaddr, LPINT RemoteSockaddrLength)
    {
        return (*m_GetAcceptExSockAddrs)(lpOutputBuffer, dwReceiveDataLength, dwLocalAddressLength, dwRemoteAddressLength, LocalSockaddr,
            LocalSockaddrLength, RemoteSockaddr, RemoteSockaddrLength);
    }

    static void IoctlInitFunc(SOCKET _socket)
    {
        GUID guidAcceptEx = WSAID_ACCEPTEX;
        GUID guidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

        DWORD dwBytes = 0;

        if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidAcceptEx, sizeof(guidAcceptEx), &m_AcceptEx, 
            sizeof(m_AcceptEx), &dwBytes, NULL, NULL) == SOCKET_ERROR)
        {
            fprintf(stderr, "Don't get AcceptEx pointer, error code : %d\n", WSAGetLastError());
            return ;
        }

        if (WSAIoctl(_socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guidGetAcceptExSockAddrs, sizeof(guidGetAcceptExSockAddrs), &m_GetAcceptExSockAddrs,
            sizeof(m_GetAcceptExSockAddrs), &dwBytes, NULL, NULL) == SOCKET_ERROR)
        {
            fprintf(stderr, "Don't get GetAcceptExSockAddrs pointer, error code : %d\n", WSAGetLastError());
            return ;
        }
    }

private:
    static LPFN_ACCEPTEX m_AcceptEx;
    static LPFN_GETACCEPTEXSOCKADDRS m_GetAcceptExSockAddrs;
};

LPFN_ACCEPTEX IoctlFuncSet::m_AcceptEx = NULL;
LPFN_GETACCEPTEXSOCKADDRS IoctlFuncSet::m_GetAcceptExSockAddrs = NULL;

#endif