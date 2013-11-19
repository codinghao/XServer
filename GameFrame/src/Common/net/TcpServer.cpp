#include "TcpServer.h"
#include "TcpConnection.h"
#include "Accpetor.h"
#include "MemAlloc.h"

TcpServer::TcpServer(Service* _service, PeerAddr& _peerAddr)
    : m_Acceptor(new Acceptor(_service, _peerAddr))
{
    m_AcceptHandler = AcceptHandler(this, &TcpServer::OnAccept);
    AsyncAccept();
    AsyncAccept();
    AsyncAccept();
    AsyncAccept();
}

TcpServer::~TcpServer()
{
    if (m_Acceptor != NULL)
        delete m_Acceptor;

    m_Acceptor = NULL;
}

void TcpServer::OnAccept(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    m_Acceptor->OnAccepted(_socket, *_buffer, _errorCode);

    //std::cout << "Accepted : " << _socket->GetIp() << " " << _socket->GetPort() << std::endl;

    TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
    m_ConnectionManager.AddTcpConnection(pConn);
    pConn->AsyncRecv();

    std::cout << pConn->GetConnId() << std::endl;

    AsyncAccept();
}

void TcpServer::AsyncAccept()
{
    m_Acceptor->AsyncAccept(static_cast<Socket*>(m_ConnectionManager.CreateTcpConnnectionFanctory()), 
        &m_AcceptHandler, Buffer((char*)MemAllocWithLockT.Alloc(((PEER_ADDR_SIZE+16)*2)), ((PEER_ADDR_SIZE+16)*2)));
}