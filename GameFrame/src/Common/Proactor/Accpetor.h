#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "Operation.h"

class Acceptor
{
public:
    Acceptor(Service* _service)
        : m_Service(_service)
    {
    }

    void AsyncAccept(AcceptHandler _handler, const Buffer& _buffer)
    {

    }

private:
    Socket*   m_Socket;
    Service*  m_Service;
};

#endif