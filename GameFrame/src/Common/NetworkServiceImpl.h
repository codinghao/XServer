#ifndef _NETWORK_SERVICE_IMPL_H_
#define _NETWORK_SERVICE_IMPL_H_

#include "ObjectPool.h"
#include <list>
   
class SocketContext;
class ConnectionSocket;
class EventHandler;
class HandlerThread;

class NetworkServiceImpl
{
public:
    typedef ObjectPool<SocketContext> ContextPoolT;
    typedef std::list<SocketContext*> ContextListT;
    typedef std::list<HandlerThread*> ThreadListT;

    NetworkServiceImpl(EventHandler* pHandler);
    ~NetworkServiceImpl();
    bool Init();
    void Run(const char* ip, const short port);
    void Stop();
    bool DoAccept(SocketContext* pContext);
    bool DoRecv(ConnectionSocket* pConn, SocketContext* pContext);
    bool DoSend(ConnectionSocket* pConn, SocketContext* pContext);
    bool DoClose(ConnectionSocket* pConn);
private:
    bool InitSocket();
    bool Bind(const char* sIP, const int iPort);
    bool Listen();
    bool InitWinFucEx();
    bool PostAccept(SocketContext* pContext);
    bool PostRecv(SocketContext* pContext);
    void StartWrokThread();
private:    
    HANDLE m_hIoCompletionPort;
    SOCKET m_AcceptSocket;

    ContextListT m_ContextList;
    EventHandler* m_pEventHandler;
    ThreadListT  m_ThreadList;
};

#endif