#ifndef _TCPCONNECTION_H_
#define _TCPCONNECTION_H_

#include <Compact.h>

class TcpConnection
{
public:
    TcpConnection(SOCKET s, const char* ip, const short port);
    virtual ~TcpConnection();
    void AsyncSend();
    void AsyncRecv();
    void AsyncConnection();

    void OnSend(SocketContext* pContext);
    void OnRecv(SocketContext* pContext);
    
    void SendMsg(Packet* pData);
private:
    SOCKET m_Socket;
    char   m_szIp[MAX_IP_LENGTH];
    short  m_Port;
    ContextListT m_ContextList;
    StreamBuffer m_InputStream;
    StreamBuffer m_OutputStream;
};

#endif