#include "AcceptorImpl.h"
#include "Service.h"
#include "Operation.h"

#define SOCKADDR_LENGTH (sizeof(SOCKADDR_IN) + 16)

AcceptorImpl::AcceptorImpl(Service* _service)
    : m_Socket(new Socket())
    , m_Service(_service)
{
    m_Service->AssociateIoCompletionPort((HANDLE)m_Socket->GetSocket());
}

bool AcceptorImpl::Bind(PeerAddr& _peerAddr)
{
    if (::bind(m_Socket->GetSocket(), _peerAddr(), sizeof(_peerAddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed bind address %s, port %d, error code %d\n", _peerAddr.Ip(), _peerAddr.Port(), WSAGetLastError());
        return false;
    }

    return true;
}

bool AcceptorImpl::Listen()
{
    if (listen(m_Socket->GetSocket(), SOMAXCONN) == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed listen, error code : %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

bool AcceptorImpl::InitAcceptEx()
{
    return m_AcceptEx.InitalizeAddress(m_Socket->GetSocket()) && m_GetSockAddr.InitalizeAddress(m_Socket->GetSocket());
}

void AcceptorImpl::AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer)
{
    DWORD dwBytes = 0;

    AcceptOperation* acceptOp = m_AcceptPool.Pop(_socket, _buffer, _handler);
    if (!m_AcceptEx(m_Socket->GetSocket(), _socket->GetSocket(), _buffer.m_Buffer, _buffer.m_Len - SOCKADDR_LENGTH*2, 
        SOCKADDR_LENGTH, SOCKADDR_LENGTH, &dwBytes, (LPOVERLAPPED)acceptOp))
    {
        DWORD errorCode = WSAGetLastError();
        if (errorCode != ERROR_IO_PENDING)
        {
            delete acceptOp;
            fprintf(stderr, "Post AcceptEx request failed, error code : %d", errorCode);
            return ;
        }
    }
}

void AcceptorImpl::GetAcceptExSockAddrs(Socket* _socket, const Buffer& _buffer)
{
    SOCKADDR_IN* remoteAddr = NULL;
    SOCKADDR_IN* localAddr = NULL;
    int remoteAddrLen = sizeof(SOCKADDR_IN);
    int localAddrLen = sizeof(SOCKADDR_IN);

    m_GetSockAddr(_buffer.m_Buffer, _buffer.m_Len - SOCKADDR_LENGTH*2,
        SOCKADDR_LENGTH, SOCKADDR_LENGTH, (LPSOCKADDR*)&localAddr, &localAddrLen, (LPSOCKADDR*)&remoteAddr, &remoteAddrLen);

    _socket->SetPeerAddr(PeerAddr(*remoteAddr));
}