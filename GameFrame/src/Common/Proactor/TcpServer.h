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
    void OnAccept(Socket* _socket, Buffer* _buffer, int _errorCode);
private:
    Acceptor* m_Acceptor;
    AcceptHandler m_AcceptHandler;
    ConnnectionManager m_ConnectionManager;
};

#endif