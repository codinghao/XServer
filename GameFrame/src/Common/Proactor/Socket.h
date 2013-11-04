#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "Operation.h"

#define SAFE_DELETE_SOCKET(s) if (s != INVALID_SOCKET) { closesocket(s); s = INVALID_SOCKET; }

class Socket
{
public:
    Socket()
    {}

    ~Socket()
    {
        SAFE_DELETE_SOCKET(s);
    }
    
    void AsyncRead(ReadHandler* _handler, const Buffer& _buffer)
    {
        DWORD flags = 0, bytes = 0;

        WSABUF buf;
        buf.buf = _buffer.m_Buffer;
        buf.len = _buffer.m_Len;

        ReadOperation *readOp = new ReadOperation(this, _buffer, _handler);
        int ret = WSARecv(m_Socket, &buf, 1, &bytes, &flags, (LPOVERLAPPED)readOp, NULL);
        if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        {
            fprintf(stderr, "Asynchronous read from socket error. Error code %d.\n", WSAGetLastError());
            delete readOp;
            return false;
        }
        return true;
    }

    void AsyncWrite(WriteHandler* _handler, const Buffer& _buffer)
    {
        DWORD flags = 0, bytes = 0;

        WSABUF buf;
        buf.buf = _buffer.m_Buffer;
        buf.len = _buffer.m_Len;

        WriteOperation *writeOp = new WriteOperation(this, _buffer, _handler);
        int ret = WSASend(m_Socket, &buf, 1, &bytes, &flags, (LPOVERLAPPED)writeOp, NULL);
        if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
        {
            fprintf(stderr, "Asynchronous write to socket error. Error code %d.\n", WSAGetLastError());
            delete writeOp;
            return false;
        }
        return true;
    }

    SOCKET GetSocket() { return m_Socket;         }
    const char* GetIp(){ return m_PeerAddr.Ip();  }
    ushort GetPort()   { return m_PeerAddr.Port();}

private:
    SOCKET   m_Socket;
    PeerAddr m_PeerAddr;
};

#endif