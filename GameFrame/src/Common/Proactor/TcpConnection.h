#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "Socket.h"

class TcpConnection : public Socket
{
public:
    TcpConnection();
    ~TcpConnection();
    
    void OnRead(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode);

    ulonglong GetConnId() { return m_ConnId; }
    ulonglong SetConnId(ulonglong _id) { return m_ConnId = _id;}
private:
    ulonglong m_ConnId;
    ReadHandler m_ReadHandler;
};

#endif