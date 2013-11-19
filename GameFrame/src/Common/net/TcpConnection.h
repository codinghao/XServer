#ifndef _TCP_CONNECTION_H_
#define _TCP_CONNECTION_H_

#include "Socket.h"

class TcpConnection : public Socket
{
public:
    TcpConnection(ReadHandler& _readHandler, WriteHandler& _writeHandler);
    ~TcpConnection();
    
    void OnRead(Buffer* _buffer);
    void OnWrite(Buffer* _buffer);

    void AsyncRecv();
    void AsyncSend();

    long GetConnId() { return m_ConnId; }
    long SetConnId(long _id) { return m_ConnId = _id;}
private:
    long m_ConnId;
    ReadHandler m_ReadHandler;
    WriteHandler m_WriteHandler;
};

#endif