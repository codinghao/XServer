#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "..\Delegate.h"

class Socket;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> AcceptHandler;

class Service;
class AcceptorImpl;
class Acceptor
{
public:
    Acceptor(Service* _service, PeerAddr& _peerAddr);
    ~Acceptor();
    void AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer);
private:
    AcceptorImpl* m_AcceptorImpl;
};

#endif