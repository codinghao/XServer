#ifndef _WIN_FUNCTOR_H_
#define _WIN_FUNCTOR_H_

#include "..\Compact.h"

class AcceptExFunctor
{
public:
    bool InitalizeAddress(SOCKET _socket)
    {
        DWORD dwBytes = 0;
        GUID guid = WSAID_ACCEPTEX;

        if (WSAIoctl(
            _socket, 
            SIO_GET_EXTENSION_FUNCTION_POINTER, 
            &guid, 
            sizeof(guid), 
            &m_Functor, 
            sizeof(m_Functor), 
            &dwBytes, 
            NULL, 
            NULL) == SOCKET_ERROR)
        {
            fprintf(stderr, "Don't get AcceptEx pointer, error code : %d\n", WSAGetLastError());
            return false;
        }

        return true;
    }
    BOOL operator ()(
        SOCKET sListenSocket, 
        SOCKET sAcceptSocket, 
        PVOID lpOutputBuffer, 
        DWORD dwReceiveDataLength, 
        DWORD dwLocalAddressLength, 
        DWORD dwRemoteAddressLength, 
        LPDWORD lpdwBytesReceived, 
        LPOVERLAPPED lpOverlapped
        )
    {
        return (*m_Functor)(
            sListenSocket, 
            sAcceptSocket, 
            lpOutputBuffer, 
            dwReceiveDataLength, 
            dwLocalAddressLength, 
            dwRemoteAddressLength, 
            lpdwBytesReceived, 
            lpOverlapped);
    }
private:
    LPFN_ACCEPTEX m_Functor;
};

class GetAcceptExSockaddrsFunctor
{
public:
    bool InitalizeAddress(SOCKET _socket)
    {
        DWORD dwBytes = 0;
        GUID guid = WSAID_GETACCEPTEXSOCKADDRS;

        if (WSAIoctl(
            _socket, 
            SIO_GET_EXTENSION_FUNCTION_POINTER, 
            &guid, 
            sizeof(guid), 
            &m_Functor,
            sizeof(m_Functor), 
            &dwBytes, 
            NULL, 
            NULL) == SOCKET_ERROR)
        {
            fprintf(stderr, "Don't get GetAcceptExSockAddrs pointer, error code : %d\n", WSAGetLastError());
            return false;
        }

        return true;
    }
    void operator ()(
        PVOID lpOutputBuffer, 
        DWORD dwReceiveDataLength, 
        DWORD dwLocalAddressLength, 
        DWORD dwRemoteAddressLength, 
        struct sockaddr **LocalSockaddr, 
        LPINT LocalSockaddrLength, 
        struct sockaddr **RemoteSockaddr, 
        LPINT RemoteSockaddrLength
        )
    {
        (*m_Functor)(
            lpOutputBuffer, 
            dwReceiveDataLength, 
            dwLocalAddressLength, 
            dwRemoteAddressLength, 
            LocalSockaddr,
            LocalSockaddrLength, 
            RemoteSockaddr, 
            RemoteSockaddrLength);
    }
private:
    LPFN_GETACCEPTEXSOCKADDRS m_Functor;
};

#endif