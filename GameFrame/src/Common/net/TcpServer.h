#ifndef _TCP_SERVER_H_
#define _TCP_SERVER_H_

#include "Socket.h"
#include "ConnectionManager.h"

class Service;
class Acceptor;
class TcpServer
{
public:
    TcpServer(Service* _service, PeerAddr& _peerAddr);
    ~TcpServer();
    void OnRead(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnAccepted(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnBreaken(Socket* _socket, Buffer* _buffer, int _errorCode);
    void AsyncAccept();
private:
    Acceptor* m_Acceptor;
    AcceptHandler m_AcceptHandler;
    ReadHandler m_ReadHandler;
    WriteHandler m_WriteHandler;
    ConnnectionManager m_ConnectionManager;
};

#endif