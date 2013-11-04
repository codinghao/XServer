#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "Operation.h"

#define SOCKADDR_LENGTH ((sizeof(SOCKADDR_IN) + 16)

class Acceptor
{
public:
    Acceptor(Service* _service)
        : m_Socket(new Socket())
        , m_Service(_service)
    {
    }

    bool Bind(const PeerAddr& _peerAddr)
    {
        if (bind(m_Socket->GetSocket(), _peerAddr(), sizeof(_peerAddr)) == SOCKET_ERROR)
        {
            fprintf(stderr, "Failed bind address %s, port %d, error code %d\n", _peerAddr.Ip(), _peerAddr.Port(), WSAGetLastError());
            return false;
        }

        return true;
    }

    bool Listen()
    {
        if (listen(m_Socket->GetSocket(), SOMAXCONN) == SOCKET_ERROR)
        {
            fprintf(stderr, "Failed listen, error code : %d\n", WSAGetLastError());
            return false;
        }

        return true;
    }

    void AsyncAccept(Socket* _socket, AcceptHandler* _handler, const Buffer& _buffer)
    {
        DWORD dwBytes = 0;

        AcceptOperation* acceptOp = new AcceptOperation(_socket, _buffer, _handler);
        if (!IoctlFuncSet::AcceptEx(m_Socket->GetSocket(), _socket->GetSocket(), _buffer.m_Buffer, _buffer.m_Len - SOCKADDR_LENGTH*2),   
            SOCKADDR_LENGTH, SOCKADDR_LENGTH, &dwBytes, (LPOVERLAPPED)acceptOp))
        {
            DWORD errorCode = WSAGetLastError();
            if (errorCode != ERROR_IO_PENDING)
            {
                delete acceptOp;
                fprintf(stderr, "Post AcceptEx request failed, error code : %d", errorCode);
                return ;
            }
        }
    }

private:
    Socket*   m_Socket;
    Service*  m_Service;
};

#endif