#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "EventHandler.h"

class Socket;
class Service;
class AcceptorImpl;
class Acceptor
{
public:
    Acceptor(Service* _service, PeerAddr& _peerAddr);
    ~Acceptor();
    void AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer);
    void OnAccepted(Socket* _socket, const Buffer& _buffer, int _errorCode);
private:
    AcceptorImpl* m_AcceptorImpl;
};

#endif