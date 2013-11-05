#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "..\Delegate.h"

class Socket;

typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> ReadHandler;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> WriteHandler;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> AcceptHandler;

class Socket
{
public:
    Socket();
    ~Socket();    
    void AsyncRead(ReadHandler* _handler, const Buffer& _buffer);
    void AsyncWrite(WriteHandler* _handler, const Buffer& _buffer);

    void SetPeerAddr(const PeerAddr& _peerAddr){ m_PeerAddr = _peerAddr; }
    SOCKET GetSocket() { return m_Socket;         }
    const char* GetIp(){ return m_PeerAddr.Ip();  }
    ushort GetPort()   { return m_PeerAddr.Port();}

private:
    SOCKET   m_Socket;
    PeerAddr m_PeerAddr;
};

#endif