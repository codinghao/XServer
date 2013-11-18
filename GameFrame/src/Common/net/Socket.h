#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "PeerAddr.h"
#include "Buffer.h"
#include "OperationManager.h"

class Socket
{
public:
    Socket();
    ~Socket();    
    bool AsyncRead(ReadHandler* _handler, const Buffer& _buffer);
    bool AsyncWrite(WriteHandler* _handler, const Buffer& _buffer);

    void Close();

    void SetPeerAddr(const PeerAddr& _peerAddr){ m_PeerAddr = _peerAddr; }
    SOCKET GetSocket() { return m_Socket;         }
    const char* GetIp(){ return m_PeerAddr.Ip();  }
    ushort GetPort()   { return m_PeerAddr.Port();}
private:
    SOCKET   m_Socket;
    PeerAddr m_PeerAddr;
};

#endif