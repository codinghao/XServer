#ifndef _NETWORK_STRUCT_H_
#define _NETWORK_STRUCT_H_

#include "Compact.h"
#include "Singleton.h"
#include "ObjectPool.h"

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
        ::memset(&m_Overlapped, 0, sizeof(OVERLAPPED));
        ::memset(&m_szBuffer, 0, MAX_BUFFER_LEN);
        m_wsabuf.buf = m_szBuffer;
        m_wsabuf.len = MAX_BUFFER_LEN;
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
        ::memset(m_szBuffer, 0, MAX_BUFFER_LEN);
    }

public:
    OVERLAPPED     m_Overlapped;                               // 每一个重叠网络操作的重叠结构
    SOCKET         m_Socket;                                   // 这个网络操作所使用的Socket
    WSABUF         m_wsabuf;                                   // WSA类型的缓冲区，用于给重叠操作传参数的
    char           m_szBuffer[MAX_BUFFER_LEN];                 // 这个是WSABUF里具体存字符的缓冲区
    NetEvent       m_NetEvent;                                 // 标识网络操作的类型
};

typedef ObjectPool<SocketContext> ContextPoolT;
typedef std::list<SocketContext*> ContextListT;

class ConnectionSocket
{
public:
    void Init(SOCKET s)
    {
        m_Socket = s;
    }

    void Destory()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            ::closesocket(m_Socket);
            m_Socket = INVALID_SOCKET; 
        }
        
        ContextListT::iterator it = m_ContextList.begin();
        for (; it != m_ContextList.end(); ++ it)
            ContextPoolT::Instance().Push(*it);

        m_ContextList.clear();
    }
public:
    SOCKET         m_Socket;
    ContextListT   m_ContextList;
};

#endif