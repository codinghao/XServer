#ifndef _MSG_HANDLER_H_
#define _MSG_HANDLER_H_

#include "EventHandler.h"
#include "TcpConnection.h"

class MsgHandler : public EventHandler
{
public:
    MsgHandler();
    ~MsgHandler();
    TcpConnection* OnAccept(const char* ip, const short port);
    void OnSend();
    void OnRecv(TcpConnection* pConn, SocketContext* pContext);
    void OnClose(TcpConnection* pConn);
};

MsgHandler::MsgHandler()
{

}

MsgHandler::~MsgHandler()
{

}

TcpConnection* MsgHandler::OnAccept(const char* ip, const short port)
{
    std::cout << "New Connection IP[" << ip << "] " << "PORT[" << port << "]"  << std::endl;
    return new TcpConnection(ip, port);
}

void MsgHandler::OnSend()
{

}

void MsgHandler::OnRecv(TcpConnection* pConn, SocketContext* pContext)
{
    std::cout << "IP[" << pConn->Ip() << "]" << "PORT[" << pConn->Port() << "], Content[" << pContext->m_szRealBuf << "]" << std::endl;
}

void MsgHandler::OnClose(TcpConnection* pConn)
{
    if (pConn == NULL) {   return ;  }

    std::cout << "Client close IP[" << pConn->Ip() << "]" << "PORT[" << pConn->Port() << "]" << std::endl;
}

#endif