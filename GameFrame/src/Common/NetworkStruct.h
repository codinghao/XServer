#ifndef _NETWORK_STRUCT_H_
#define _NETWORK_STRUCT_H_

#include "Compact.h"
#include "Singleton.h"
#include "ObjectPool.h"
#include <list>

#define MAX_BUFFER_LEN 4096
#define MAX_POST_ACCEPT 10

enum NetEvent
{
    NET_ACCEPT,  //接受连接
    NET_SEND,    //发送数据
    NET_RECV,    //接收数据
    NET_CLOSE,   //客户端断开
    NET_MAX      //最大操作码
};	

class SocketContext
{
public:
    void Init(SOCKET s, NetEvent event)
    {
        m_Socket = s;
        m_NetEvent = event;
        m_WsaBuf.buf = m_szRealBuf;
        m_WsaBuf.len = MAX_BUFFER_LEN;
        ::memset(&m_Overlapped, 0, sizeof(OVERLAPPED));
        ::memset(m_szRealBuf, 0, sizeof(m_szRealBuf));
    }

    void Destory()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            ::closesocket(m_Socket);
            m_Socket = INVALID_SOCKET;
        }
    }

    void ResetBuffer()
    {
        ::memset(m_szRealBuf, 0, sizeof(m_szRealBuf));
    }
public:
    OVERLAPPED     m_Overlapped; 
    SOCKET         m_Socket;     
    WSABUF         m_WsaBuf;
    char           m_szRealBuf[MAX_BUFFER_LEN];
    NetEvent       m_NetEvent; 
};

typedef ObjectPool<SocketContext> ContextPoolT;
typedef std::list<SocketContext*> ContextListT;

#endif