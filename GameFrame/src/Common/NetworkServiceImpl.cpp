#include "NetworkServiceImpl.h"
#include "EventHandler.h"
#include "HandlerThread.h"
#include "NetworkStruct.h"
#include "TcpConnection.h"

#include <MSWSock.h>

#pragma comment(lib,"ws2_32.lib")

LPFN_ACCEPTEX  g_AcceptEx = NULL;
LPFN_GETACCEPTEXSOCKADDRS g_GetAcceptExSockAddrs = NULL;

NetworkServiceImpl::NetworkServiceImpl(EventHandler* pHandler)
    : m_pEventHandler(pHandler)
    , m_AcceptSocket(INVALID_SOCKET)
{

}

NetworkServiceImpl::~NetworkServiceImpl()
{
    Stop();
}

bool NetworkServiceImpl::Init()
{
    WSADATA wsaData;
    int ret= WSAStartup(MAKEWORD(2,2), &wsaData);
    if (NO_ERROR != ret)
    {
        fprintf(stderr, "Failed call WSAStartup function.");
        return false;
    }

    m_hIoCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (m_hIoCompletionPort == NULL)
    {
        fprintf(stderr, "Failed create IO completion port. Error code : %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

void NetworkServiceImpl::Run(const char* ip, const short port)
{
    if (!(InitSocket() && Bind(ip, port) && Listen() && InitWinFucEx()))
    {
        return ;
    }

    for (int i = 0; i < MAX_POST_ACCEPT; ++ i)
    {
        SocketContext* pContext = ContextPoolT::Instance().Pop();
        if (PostAccept(pContext) == false) 
        {
            pContext->Destory();
            ContextPoolT::Instance().Push(pContext);
            pContext = NULL;
            break;
        }
        m_ContextList.push_back(pContext);
    }

    StartWrokThread();
}

void NetworkServiceImpl::Stop()
{
    if (m_hIoCompletionPort != NULL)
    {
        ::CloseHandle(m_hIoCompletionPort);
        m_hIoCompletionPort = NULL;
    }

    if (m_AcceptSocket != NULL)
    {
        ::closesocket(m_AcceptSocket);
        m_AcceptSocket = INVALID_SOCKET;
    }

    for (ContextListT::iterator it = m_ContextList.begin(); it != m_ContextList.end();)
    {
        ContextPoolT::Instance().Push(*it);
        it = m_ContextList.erase(it);
    }

    for (ThreadListT::iterator it = m_ThreadList.begin(); it != m_ThreadList.end();)
    {
        HandlerThread *pThread = *it;
        pThread->Stop();
        pThread->Join();
        it = m_ThreadList.erase(it);
        delete pThread;
    }
}

bool NetworkServiceImpl::InitSocket()
{
    m_AcceptSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (m_AcceptSocket == INVALID_SOCKET)
    {
        fprintf(stderr, "Failed init socket, error code : %d\n", WSAGetLastError());
        return false;
    }

    if (CreateIoCompletionPort((HANDLE)m_AcceptSocket, m_hIoCompletionPort, NULL, 0) == NULL)
    {
        fprintf(stderr, "Association socket, error code : %d\n", WSAGetLastError());
    }

    return true;
}

bool NetworkServiceImpl::InitWinFucEx()
{
    GUID guidAcceptEx = WSAID_ACCEPTEX;
    GUID guidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;
    DWORD dwBytes = 0;
    if (WSAIoctl(
        m_AcceptSocket, 
        SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidAcceptEx,
        sizeof(guidAcceptEx),
        &g_AcceptEx,
        sizeof(g_AcceptEx),
        &dwBytes,
        NULL,
        NULL) == SOCKET_ERROR)
    {
        fprintf(stderr, "Don't get AcceptEx pointer, error code : %d\n", WSAGetLastError());
        return false;
    }

    if (WSAIoctl(
        m_AcceptSocket,
        SIO_GET_EXTENSION_FUNCTION_POINTER,
        &guidGetAcceptExSockAddrs,
        sizeof(guidGetAcceptExSockAddrs),
        &g_GetAcceptExSockAddrs,
        sizeof(g_GetAcceptExSockAddrs),
        &dwBytes,
        NULL,
        NULL) == SOCKET_ERROR)
    {
        fprintf(stderr, "Don't get GetAcceptExSockAddrs pointer, error code : %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

bool NetworkServiceImpl::Bind(const char* sIP, const int iPort)
{
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::inet_addr(sIP);
    serverAddr.sin_port = ::htons(iPort);

    if (bind(m_AcceptSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed bind address : %s, port : %d, error code : %d\n", sIP, iPort, WSAGetLastError());
        return false;
    }

    return true;
}

bool NetworkServiceImpl::Listen()
{
    if (listen(m_AcceptSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        fprintf(stderr, "Failed listen, error code : %d\n", WSAGetLastError());
        return false;
    }

    return true;
}

void NetworkServiceImpl::StartWrokThread()
{
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    uint i = 0;
    for (; i < si.dwNumberOfProcessors * 2; ++ i)
    {
        HandlerThread* pThread = new HandlerThread(this);
        pThread->Start();
        m_ThreadList.push_back(pThread);
    }
}

bool NetworkServiceImpl::PostAccept(SocketContext* pContext)
{
    DWORD dwBytes = 0;
    pContext->Init(WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED), NET_ACCEPT);
    if (pContext->m_Socket == INVALID_SOCKET)
    {
        fprintf(stderr, "Create accept socket failed, error code : %d", WSAGetLastError());
        return false;
    }

    if (g_AcceptEx(m_AcceptSocket, pContext->m_Socket, pContext->m_WsaBuf.buf, pContext->m_WsaBuf.len - ((sizeof(SOCKADDR_IN)+16)*2),   
        sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, &pContext->m_Overlapped) == false)
    {
        if (WSAGetLastError() != ERROR_IO_PENDING)
        {
            fprintf(stderr, "Post AcceptEx request failed, error code : %d", WSAGetLastError());
            return false;
        }
    }

    return true;
}

bool NetworkServiceImpl::DoAccept(SocketContext* pContext)
{
    SOCKADDR_IN* clientAddr = NULL;
    SOCKADDR_IN* localAddr = NULL;
    int iRemonteAddrLen = sizeof(SOCKADDR_IN);
    int iLocalAddrLen = sizeof(SOCKADDR_IN);

    g_GetAcceptExSockAddrs(pContext->m_WsaBuf.buf, pContext->m_WsaBuf.len - ((sizeof(SOCKADDR_IN) + 16)*2),
        sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&localAddr, &iLocalAddrLen, (LPSOCKADDR*)&clientAddr, &iRemonteAddrLen);
    
    TcpConnection* pConn = m_pEventHandler->OnAccept(inet_ntoa(clientAddr->sin_addr), ntohs(clientAddr->sin_port));
    pConn->m_Socket = pContext->m_Socket;

    HANDLE hRet = CreateIoCompletionPort((HANDLE)pConn->m_Socket, m_hIoCompletionPort, (DWORD)pConn, 0);
    if (hRet == NULL)
    {
        fprintf(stderr, "Call CreateIoCompletionPort() failed, error code : %d", GetLastError());
        return false;
    }

    if (pContext->m_WsaBuf.len - ((sizeof(SOCKADDR_IN) + 16)*2) > 0)
    {
        pContext->m_WsaBuf.buf = pContext->m_szRealBuf + ((sizeof(SOCKADDR_IN) + 16)*2);
        m_pEventHandler->OnRecv(pConn, pContext);
    }

    SocketContext* pRecvContext = ContextPoolT::Instance().Pop();
    pRecvContext->Init(pConn->m_Socket, NET_RECV);
    if (!PostRecv(pRecvContext))
    {
        pRecvContext->Destory();
        ContextPoolT::Instance().Push(pRecvContext);
        pRecvContext = NULL;
        return false;
    }
    pConn->m_ContextList.push_back(pRecvContext);

    return PostAccept(pContext);
}

bool NetworkServiceImpl::PostRecv(SocketContext* pContext)
{
    DWORD dwFlags = 0;
    DWORD dwBytes = 0;

    pContext->ResetBuffer();

    int nBytesRecv = WSARecv(pContext->m_Socket, &pContext->m_WsaBuf, 1, &dwBytes, &dwFlags, &pContext->m_Overlapped, NULL);
    if (nBytesRecv == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        fprintf(stderr, "Post recv failed!");
        return false;
    }

    return true;
}

bool NetworkServiceImpl::DoRecv(TcpConnection* pConn, SocketContext* pContext)
{
    m_pEventHandler->OnRecv(pConn, pContext);

    return PostRecv(pContext);
}

bool NetworkServiceImpl::DoSend(TcpConnection* pConn, SocketContext* pContext)
{
    m_pEventHandler->OnSend();

    return true;
}

bool NetworkServiceImpl::DoClose(TcpConnection* pConn)
{
    m_pEventHandler->OnClose(pConn);
    delete pConn;

    return true;
}