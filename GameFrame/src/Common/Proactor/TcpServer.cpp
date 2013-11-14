#include "TcpServer.h"
#include "TcpConnection.h"
#include "Accpetor.h"
#include "MemAlloc.h"

TcpServer::TcpServer(Service* _service, PeerAddr& _peerAddr)
    : m_Acceptor(new Acceptor(_service, _peerAddr))
{
    m_AcceptHandler = AcceptHandler(this, &TcpServer::OnAccept);
    m_Acceptor->AsyncAccept(static_cast<Socket*>(m_ConnectionManager.CreateTcpConnnectionFanctory()), 
        &m_AcceptHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
}

TcpServer::~TcpServer()
{
    if (m_Acceptor != NULL)
        delete m_Acceptor;

    m_Acceptor = NULL;
}

void TcpServer::OnAccept(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    MemAllocT.Dealloc(_buffer->m_Buffer, _buffer->m_MaxLen);

    m_Acceptor->OnAccepted(_socket, *_buffer, _errorCode);

    std::cout << "Accepted : " << _socket->GetIp() << " " << _socket->GetPort() << std::endl;

    TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
    m_ConnectionManager.AddTcpConnection(pConn);
    pConn->OnRead(_socket, _buffer, _errorCode);

    m_Acceptor->AsyncAccept(static_cast<Socket*>(m_ConnectionManager.CreateTcpConnnectionFanctory()), 
        &m_AcceptHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
}