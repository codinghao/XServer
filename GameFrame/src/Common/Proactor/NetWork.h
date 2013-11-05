#ifndef _NETWORK_H_
#define _NETWORK_H_

#define BUFFER_SIZE 4096

#include "Socket.h"
#include "Accpetor.h"

class TcpConnection : public Socket
{
public:
    TcpConnection()
    {
        m_ReadHandler = ReadHandler(this, &TcpConnection::OnRead);
    }

    void OnRead(Socket* _socket, Buffer* _buffer, int _errorCode)
    {

    }
private:
    ReadHandler m_ReadHandler;
};

class TcpServer
{
public:
    TcpServer(Service* _service, PeerAddr& _peerAddr)
        : m_Acceptor(new Acceptor(_service, _peerAddr))
    {
        m_AcceptHandler = AcceptHandler(this, &TcpServer::OnAccept);
        m_Acceptor->AsyncAccept((Socket*)new TcpConnection(), &m_AcceptHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
    }

    void OnAccept(Socket* _socket, Buffer* _buffer, int _errorCode)
    {

    }

private:
    Acceptor* m_Acceptor;
    AcceptHandler m_AcceptHandler;
};

#endif