#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "Socket.h"

class TcpConnection : public Socket
{
public:
    TcpConnection();
    
    void OnRead(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode);

    uint GetConnId() { return m_ConnId; }
    uint SetConnId(uint _id) { return m_ConnId = _id;}
private:
    uint m_ConnId;
    ReadHandler m_ReadHandler;
};

#endif