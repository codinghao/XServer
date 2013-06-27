#ifndef _SERVER_SOCKET_H_
#define _SERVER_SOCKET_H_

#include "Compact.h"
#include <winsock2.h>
#include <MSWSock.h>
#include <list>
#pragma comment(lib,"ws2_32.lib")

#define MAX_BUFFER_LEN 4096
#define MAX_POST_ACCEPT 10

typedef struct _IoContext
{

   	OVERLAPPED     m_Overlapped;                               // ÿһ���ص�����������ص��ṹ(���ÿһ��Socket��ÿһ����������Ҫ��һ��)              
	SOCKET         m_Socket;                                   // ������������ʹ�õ�Socket
	WSABUF         m_wsabuf;                                   // WSA���͵Ļ����������ڸ��ص�������������
	char           m_szBuffer[MAX_BUFFER_LEN];                 // �����WSABUF�������ַ��Ļ�����
	NetEvent       m_NetEvent;                                 // ��ʶ�������������(��Ӧ�����ö��)

    _IoContext()
    {
        ::memset(&m_Overlapped, 0, sizeof(OVERLAPPED));
        ::memset(&m_szBuffer, 0, MAX_BUFFER_LEN);
        m_Socket = INVALID_SOCKET;
        m_wsabuf.buf = m_szBuffer;
        m_wsabuf.len = MAX_BUFFER_LEN;
        m_NetEvent = NET_NULL;
    }

    ~_IoContext()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            ::closesocket(m_Socket);
            m_Socket = INVALID_SOCKET;
        }
    }

	void ResetBuffer()
	{
		memset(m_szBuffer, 0, MAX_BUFFER_LEN);
	}

}IoContext;

typedef std::list<IoContext*>  IoContextList_t;

typedef struct _ClientContext
{
    SOCKET       m_Socket;
    SOCKADDR_IN  m_ClientAddr;
    IoContextList_t m_ContextList;  //��ÿ���ͻ���socket����Ͷ�ݶ��IO����

    _ClientSocket()
    {
        m_Socket = INVALID_SOCKET;
        ::memset(SOCKADDR_IN, 0, sizeof(SOCKADDR_IN));
    }

    ~_ClientSocket()
    {
        if (m_Socket != INVALID_SOCKET)
        {
            ::closesocket(m_Socket);
            m_Socket = INVALID_SOCKET; 
        }

        for (IoContextList_t::iterator it = m_ContextList.begin(); it != m_ContextList.end(); ++ it)
        {
            IoContext* pContext = *it;
            it = it->erase(it);
            delete pContext;
        }
    }

    IoContext* GetNewIoContext()
    {
        IoContext* pContext = new IoContext();
        m_ContextList.push_back(pContext);
        return pContext;
    }

    void RemoveContext(IoContext* pContext)
    {
        for (IoContextList_t::iterator it = m_ContextList.begin(); it != m_ContextList.end(); ++ it)
        {
            if (*it == pContext)
            {
                delete *it;
                break;
            }
        }
    }
}ClientContext;

typedef std::list<ClientContext> ClientContextList_t;

class ServerSocket
{
public:
    ServerSocket();
    virtual ~ServerSocket();
    bool Init();
    void Clear();
	bool Listen();
	bool PostAccept(IoContext* pAcceptIoContext);
	bool DoAccept(ClientContext* pSocketContext, IoContext* pIoContext);
	bool PostRecv(IoContext* pIoContext);
	bool DoRecv(ClientContext* pSocketContext, IoContext* pIoContext);
private:
    HANDLE m_hIoCompletionPort;
    ClientContextList_t  m_ClientContext;            // �ͻ���Socket��Context��Ϣ        
	ClientContext*       m_pListenContext;           // ���ڼ�����Socket��Context��Ϣ
	LPFN_ACCEPTEX                m_lpfnAcceptEx; // AcceptEx �� GetAcceptExSockaddrs �ĺ���ָ�룬���ڵ�����������չ����
	LPFN_GETACCEPTEXSOCKADDRS    m_lpfnGetAcceptExSockAddrs; 
};


class SocketThread : public Thread
{
public:
	SocketThread(ServerSocket& sSocket)
		: m_ServerSocket(sSocket)
	{}

	virtual ~SocketThread() {}

	void Run();

private:
	ServerSocket& m_ServerSocket;
};

#endif