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

   	OVERLAPPED     m_Overlapped;                               // 每一个重叠网络操作的重叠结构(针对每一个Socket的每一个操作，都要有一个)              
	SOCKET         m_Socket;                                   // 这个网络操作所使用的Socket
	WSABUF         m_wsabuf;                                   // WSA类型的缓冲区，用于给重叠操作传参数的
	char           m_szBuffer[MAX_BUFFER_LEN];                 // 这个是WSABUF里具体存字符的缓冲区
	NetEvent       m_NetEvent;                                 // 标识网络操作的类型(对应上面的枚举)

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
    IoContextList_t m_ContextList;  //对每个客户端socket可以投递多个IO请求

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
    ClientContextList_t  m_ClientContext;            // 客户端Socket的Context信息        
	ClientContext*       m_pListenContext;           // 用于监听的Socket的Context信息
	LPFN_ACCEPTEX                m_lpfnAcceptEx; // AcceptEx 和 GetAcceptExSockaddrs 的函数指针，用于调用这两个扩展函数
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