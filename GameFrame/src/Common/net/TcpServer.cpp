#include "TcpServer.h"
#include "TcpConnection.h"
#include "Accpetor.h"
#include "MemAlloc.h"

TcpServer::TcpServer(Service* _service, PeerAddr& _peerAddr)
    : m_Acceptor(new Acceptor(_service, _peerAddr))
{
    m_AcceptHandler = AcceptHandler(this, &TcpServer::OnAccepted);
    m_ReadHandler   = ReadHandler  (this, &TcpServer::OnRead);
    m_WriteHandler  = WriteHandler (this, &TcpServer::OnWrite);

    AsyncAccept();
}

TcpServer::~TcpServer()
{
    if (m_Acceptor != NULL)
        delete m_Acceptor;

    m_Acceptor = NULL;
}

void TcpServer::OnAccepted(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    m_Acceptor->OnAccepted(_socket, *_buffer, _errorCode);

    //std::cout << "Accepted : " << _socket->GetIp() << " " << _socket->GetPort() << std::endl;

    TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
    m_ConnectionManager.AddTcpConnection(pConn);
    pConn->AsyncRecv();

    std::cout << pConn->GetConnId() << std::endl;

    MemAllocWithLockT.Dealloc(_buffer->m_Buffer, _buffer->m_MaxLen);

    AsyncAccept();
}

void TcpServer::OnRead(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    if (_errorCode != 0)
    {
        OnBreaken(_socket, _buffer, _errorCode);
    }
    else
    {
        TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
        pConn->OnRead(_buffer);
    }
}

void TcpServer::OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}

void TcpServer::OnBreaken(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
    if (m_ConnectionManager.FindAndRemoveConnection(pConn))
    {
        std::cout << "Connnection breaken conn id : " << pConn->GetConnId() << std::endl;

        m_ConnectionManager.DeleteTcpConnection(pConn);
    }
}

void TcpServer::AsyncAccept()
{
    m_Acceptor->AsyncAccept(static_cast<Socket*>(m_ConnectionManager.CreateTcpConnnectionFanctory(m_ReadHandler, m_WriteHandler)), 
        &m_AcceptHandler, Buffer((char*)MemAllocWithLockT.Alloc(((PEER_ADDR_SIZE+16)*2)), ((PEER_ADDR_SIZE+16)*2)));
}