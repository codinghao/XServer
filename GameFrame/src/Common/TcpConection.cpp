#include "TcpConnection.h"

TcpConnection::TcpConnection(SOCKET s, const char* ip, const short port)
    : m_Socket(s)
    , m_Port(port)
{
    ::strcpy(m_szIp, ip);
}

virtual TcpConnection::~TcpConnection()
{
    if (m_Socket != INVALID_SOCKET)
        closesocket(m_Socket);

    ContextListT::iterator it = m_ContextList.begin();
    for (; it != m_ContextList.end(); ++ it)
        ContextPoolT::Instance().Push(*it);

    m_ContextList.clear();
}

void TcpConnection::AsyncSend()
{
    DWORD dwFlags = 0;
    DWORD dwBytes = 0;

    SocketContext* pContext = ContextPoolT::Instance().Pop();
    pContext->Init(m_Socket, NET_SEND);

    m_OutputStream.Read(pContext->m_WsaBuf, pContext->m_WsaBuf.len);

    int nBytesRecv = WSASend(m_Socket, &pContext->m_WsaBuf, 1, &dwBytes, &dwFlags, &pContext->m_Overlapped, NULL);
    if (nBytesRecv == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        fprintf(stderr, "Post recv failed!");
        return ;
    }
}

void TcpConnection::AsyncRecv()
{
    DWORD dwFlags = 0;
    DWORD dwBytes = 0;

    SocketContext* pContext = ContextPoolT::Instance().Pop();
    pContext->Init(m_Socket, NET_RECV);

    int nBytesRecv = WSARecv(m_Socket, &pContext->m_WsaBuf, 1, &dwBytes, &dwFlags, &pContext->m_Overlapped, NULL);
    if (nBytesRecv == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        fprintf(stderr, "Post recv failed!");
        return ;
    }
}

void TcpConnection::AsyncConnection()
{

}

void TcpConnection::OnSend(SocketContext* pContext)
{
    AsyncSend();
}

void TcpConnection::OnRecv(SocketContext* pContext)
{
    m_InputStream.Write(pContext->m_WsaBuf.buf, pContext->m_WsaBuf.len);
    AsyncRecv();
}

void TcpConnection::SendMsg(Packet* pData)
{
    m_OutputStream.Write(Packet* pData);
    AsyncSend();
}