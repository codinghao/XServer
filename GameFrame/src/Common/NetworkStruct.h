#ifndef _NETWORK_STRUCT_H_
#define _NETWORK_STRUCT_H_

#include "Compact.h"
#include "Singleton.h"
#include "ObjectPool.h"

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
    OVERLAPPED     m_Overlapped;                               // ÿһ���ص�����������ص��ṹ
    SOCKET         m_Socket;                                   // ������������ʹ�õ�Socket
    WSABUF         m_wsabuf;                                   // WSA���͵Ļ����������ڸ��ص�������������
    char           m_szBuffer[MAX_BUFFER_LEN];                 // �����WSABUF�������ַ��Ļ�����
    NetEvent       m_NetEvent;                                 // ��ʶ�������������
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