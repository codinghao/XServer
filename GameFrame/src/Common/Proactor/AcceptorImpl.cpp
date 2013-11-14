#include "AcceptorImpl.h"
#include "Service.h"
#include "Operation.h"

#define SOCKADDR_LENGTH (sizeof(sockaddr_in) + 16)

AcceptorImpl::AcceptorImpl(Service* _service)
    : m_Socket(new Socket())
    , m_Service(_service)
{
    m_Service->AssociateIoCompletionPort((HANDLE)m_Socket->GetSocket());
}

AcceptorImpl::~AcceptorImpl()
{
    if (m_Socket != NULL)
        delete m_Socket;
    m_Socket = NULL;
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

    Operation* acceptOp = gs_OperationManager.CreateAcceptOperationFactory(_socket, _buffer, _handler);
    if (!m_AcceptEx(m_Socket->GetSocket(), _socket->GetSocket(), _buffer.m_Buffer, _buffer.m_MaxLen - ((PEER_ADDR_SIZE+16)*2), 
        (PEER_ADDR_SIZE+16), (PEER_ADDR_SIZE+16), &dwBytes, (LPOVERLAPPED)acceptOp))
    {
        DWORD errorCode = WSAGetLastError();
        if (errorCode != ERROR_IO_PENDING)
        {
            gs_OperationManager.OnDestory(acceptOp, errorCode);
            fprintf(stderr, "Post AcceptEx request failed, error code : %d", errorCode);
            return ;
        }
    }
}

void AcceptorImpl::OnAccepted(Socket* _socket, const Buffer& _buffer, int _errorCode)
{
    m_Service->AssociateIoCompletionPort((HANDLE)_socket->GetSocket());

    sockaddr_in* remoteAddr = NULL;
    sockaddr_in* localAddr = NULL;
    int remoteAddrLen = PEER_ADDR_SIZE;
    int localAddrLen = PEER_ADDR_SIZE;

    m_GetSockAddr(_buffer.m_Buffer, _buffer.m_MaxLen - (PEER_ADDR_SIZE+16)*2,
        (PEER_ADDR_SIZE+16), (PEER_ADDR_SIZE+16), (LPSOCKADDR*)&localAddr, &localAddrLen, (LPSOCKADDR*)&remoteAddr, &remoteAddrLen);

    _socket->SetPeerAddr(PeerAddr(*remoteAddr));
}