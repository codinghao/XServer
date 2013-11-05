#include "Accpetor.h"
#include "AcceptorImpl.h"

Acceptor::Acceptor(Service* _service, PeerAddr& _peerAddr)
    : m_AcceptorImpl(new AcceptorImpl(_service))
{    
    m_AcceptorImpl->Bind(_peerAddr);
    m_AcceptorImpl->Listen();
    m_AcceptorImpl->InitAcceptEx();
}

Acceptor::~Acceptor()
{
}

void Acceptor::AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer)
{
    m_AcceptorImpl->AsyncAccept(_socket, _handler, _buffer);
}