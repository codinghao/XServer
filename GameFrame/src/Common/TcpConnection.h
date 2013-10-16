#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include <Compact.h>
#include "Message.h"
#include "../Common/StreamBuffer.h"
#include "NetworkStruct.h"

class TcpConnection
{
public:
    TcpConnection(const char* ip, const short port);
    virtual ~TcpConnection();
    void AsyncSend();
    void AsyncRecv();
    void AsyncConnection();

    void OnSend(SocketContext* pContext);
    void OnRecv(SocketContext* pContext);
    
    void SendMsg(Packet* pData);

    const char* Ip() { return m_szIp; }

public:
    SOCKET m_Socket;    
    ContextListT m_ContextList;
private:
    char   m_szIp[16];
    short  m_Port;

    StreamBuffer m_InputStream;
    StreamBuffer m_OutputStream;
};

#endif