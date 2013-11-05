#include "Socket.h"
#include "Operation.h"

#define SAFE_DELETE_SOCKET(s) if (s != INVALID_SOCKET) { closesocket(s); s = INVALID_SOCKET; }

Socket::Socket()
    : m_Socket(INVALID_SOCKET) 
{
    m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_Socket == INVALID_SOCKET)
    {
        fprintf(stderr, "Create socket failed, error code %d.\n", WSAGetLastError());
    }
}

Socket::~Socket()
{
    SAFE_DELETE_SOCKET(m_Socket);
}

void Socket::AsyncRead(ReadHandler* _handler, const Buffer& _buffer)
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
    }
}

void Socket::AsyncWrite(WriteHandler* _handler, const Buffer& _buffer)
{
    DWORD flags = 0, bytes = 0;

    WSABUF buf;
    buf.buf = _buffer.m_Buffer;
    buf.len = _buffer.m_Len;

    WriteOperation *writeOp = new WriteOperation(this, _buffer, _handler);
    int ret = WSASend(m_Socket, &buf, 1, &bytes, flags, (LPOVERLAPPED)writeOp, NULL);
    if (ret == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        fprintf(stderr, "Asynchronous write to socket error. Error code %d.\n", WSAGetLastError());
        delete writeOp;
    }
}