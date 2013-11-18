#ifndef _ACCEPTIMPL_H_
#define _ACCEPTIMPL_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "Socket.h"
#include "WinFunctor.h"

class Service;
class AcceptOperation;
class AcceptorImpl
{
public:
    AcceptorImpl(Service* _service);
    ~AcceptorImpl();
    bool Bind(PeerAddr& _peerAddr);
    bool Listen();
    bool InitAcceptEx();
    void AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer);
    void OnAccepted(Socket* _socket, const Buffer& _buffer, int _errorCode);
private:
    Socket*   m_Socket;
    Service*  m_Service;
    AcceptExFunctor m_AcceptEx;
    GetAcceptExSockaddrsFunctor m_GetSockAddr;
};

#endif