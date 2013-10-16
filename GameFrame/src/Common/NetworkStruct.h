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
    NET_ACCEPT,  //��������
    NET_SEND,    //��������
    NET_RECV,    //��������
    NET_CLOSE,   //�ͻ��˶Ͽ�
    NET_MAX      //��������
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