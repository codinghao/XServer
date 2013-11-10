#include "TcpServer.h"

TcpServer::TcpServer(Service* _service, PeerAddr& _peerAddr)
    : m_Acceptor(new Acceptor(_service, _peerAddr))
{
    m_AcceptHandler = AcceptHandler(this, &TcpServer::OnAccept);
    m_Acceptor->AsyncAccept((Socket*)m_ConnPool.Pop(), &m_AcceptHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
}

TcpServer::~TcpServer()
{
    if (m_Acceptor != NULL)
        delete m_Acceptor;
    
    m_Acceptor = NULL;
}

void TcpServer::OnAccept(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    m_Acceptor->GetAcceptExSockAddrs(_buffer, _socket);

    TcpConnection* pConn = (TcpConnection*)_socket;
    pConn->OnRead(_socket, _buffer)
}