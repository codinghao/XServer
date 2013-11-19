#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "Socket.h"

class TcpConnection : public Socket
{
public:
    TcpConnection(BreakenHandler& _breakenHandler);
    ~TcpConnection();
    
    void OnRead(Socket* _socket, Buffer* _buffer, int _errorCode);
    void OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode);

    void AsyncRecv();
    void AsyncSend();

    long GetConnId() { return m_ConnId; }
    long SetConnId(long _id) { return m_ConnId = _id;}
private:
    long m_ConnId;
    ReadHandler m_ReadHandler;
    WriteHandler m_WriteHandler;
    BreakenHandler m_BreakenHandler;
    long volatile m_PendingReadCount;
    long volatile m_PendingWriteCount;
};

#endif