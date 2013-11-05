#ifndef _ACCEPTIMPL_H_
#define _ACCEPTIMPL_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "Socket.h"
#include "WinFunctor.h"

class Service;
class AcceptorImpl
{
public:
    AcceptorImpl(Service* _service);
    bool Bind(PeerAddr& _peerAddr);
    bool Listen();
    bool InitAcceptEx();
    void AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer);
private:
    Socket*   m_Socket;
    Service*  m_Service;
    AcceptExFunctor m_AcceptEx;
};

#endif