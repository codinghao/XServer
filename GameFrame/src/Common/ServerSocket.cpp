#include "ServerSocket.h"

ServerSocket::ServerSocket()
    : m_hIoCompletionPort(NULL)
    , m_pListenContext(NULL)
    , m_lpfnAcceptEx(NULL)
    , m_lpfnGetAcceptExSockAddrs(NULL)
{

}

ServerSocket::~ServerSocket()
{
    Clear();
}

bool ServerSocket::Init()
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

void ServerSocket::Clear()
{
    if (m_hIoCompletionPort != NULL)
    {
        ::CloseHandle(m_hIoCompletionPort);
        m_hIoCompletionPort = NULL;
    }

    if (m_pListenContext != NULL)
    {
        delete m_pListenContext;
        m_pListenContext = NULL;
    }
}

bool ServerSocket::Listen()
{
	GUID guidAcceptEx = WSAID_ACCEPTEX;
	GUID guidGetAcceptExSockAddrs = WSAID_GETACCEPTEXSOCKADDRS;

	struct sockaddr_in serverAddress;
	m_pListenContext = new IoContext;

	m_pListenContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_pListenContext->m_Socket == INVALID_SOCKET)
	{
		fprintf(stderr, "Failed init socket, error code : %d\n", WSAGetLastError());
		return false;
	}

	::memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(SERVERIP);
	serverAddress.sin_port = htons(SERVERPORT);

	if (bind(m_pListenContext->m_Socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
	{
		fprintf(stderr, "Failed bind address : %s, port : %d, error code : %d\n", SERVERIP, SERVERPORT, WSAGetLastError());
		return false;
	}

	if (listen(m_pListenContext->m_Socket, SOMAXCONN) == SOCKET_ERROR)
	{
		fprintf(stderr, "Failed listen, error code : %d\n", WSAGetLastError());
		return false;
	}

	DWORD dwBytes = 0;
	if (WSAIoctl(
		m_pListenContext->m_Socket, 
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx,
		sizeof(guidAcceptEx),
		&m_lpfnAcceptEx,
		sizeof(m_lpfnAcceptEx),
		&dwBytes,
		NULL,
		NULL) == SOCKET_ERROR)
	{
		sprintf(stderr, "Don't get AcceptEx pointer, error code : %d\n", WSAGetLastError());
		return false;
	}

	if (WSAIoctl(
		m_pListenContext->m_Socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidGetAcceptExSockAddrs,
		sizeof(guidGetAcceptExSockAddrs),
		&m_lpfnGetAcceptExSockAddrs,
		sizeof(m_lpfnGetAcceptExSockAddrs),
		&dwBytes,
		NULL,
		NULL) == SOCKET_ERROR)
	{
		fprintf("Don't get GetAcceptExSockAddrs pointer, error code : %d\n", WSAGetLastError());
		return false;
	}

	for (int i = 0; i < MAX_POST_ACCEPT; ++ i)
	{
		ClientContext* pAcceptIoContext = m_pListenContext->GetNewIoContext();
		
		if (PostAccept(pAcceptIoContext) == false)
		{
			m_pListenContext->RemoveContext(pAcceptIoContext);
			return false;
		}
	}

	return true;
}

bool ServerSocket::PostAccept(IoContext* pAcceptIoContext)
{
	DWORD dwBytes = 0;
	pAcceptIoContext->m_NetEvent = IoContext::NET_ACCEPT;
	WSABUF *pBuf = &pAcceptIoContext->m_wsabuf;
	OVERLAPPED *pOverLapped = &pAcceptIoContext->m_Overlapped;

	pAcceptIoContext->m_Socket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (pAcceptIoContext->m_Socket == INVALID_SOCKET)
	{
		fprintf(stderr, "Create accept socket failed, error code : %d", WSAGetLastError());
		return false;
	}

	if (m_lpfnAcceptEx(m_pListenContext->m_Socket, pAcceptIoContext->m_Socket, pBuf->buf, pBuf->len - ((sizeof(SOCKADDR_IN)+16)*2),   
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwBytes, pOverLapped) == false)
	{
		if (WSAGetLastError() != ERROR_IO_PENDING)
		{
			fprintf(stderr, "Post AcceptEx request failed, error code : %d", WSAGetLastError());
			return false;
		}
	}

	return true;
}

bool ServerSocket::DoAccept(ClientContext* pSocketContext, IoContext* pIoContext)
{
	SOCKADDR_IN* clientAddr = NULL;
	SOCKADDR_IN* localAddr = NULL;
	int iRemonteAddrLen = sizeof(SOCKADDR_IN);
	int iLocalAddrLen = sizeof(SOCKADDR_IN);

	m_lpfnGetAcceptExSockAddrs(pIoContext->m_wsabuf.buf, pIoContext->m_wsabuf.len - ((sizeof(SOCKADDR_IN) + 16)*2),
		sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (LPSOCKADDR*)&localAddr, &iLocalAddrLen, (LPSOCKADDR*)&iRemonteAddrLen, &iRemonteAddrLen);

	ClientContext* pClientContext = new ClientContext;
	pClientContext->m_Socket = pIoContext->m_Socket;
	memcpy(&(pClientContext->m_ClientAddr), clientAddr, sizeof(SOCKADDR_IN));

	HANDLE hRet = CreateIoCompletionPort((HANDLE)pClientContext->m_Socket, m_hIoCompletionPort, (DWORD)pClientContext, 0);
	if (hRet == NULL)
	{
		fprintf(stderr, "Call CreateIoCompletionPort() failed, error code : %d", GetLastError());
		return false;
	}

	IoContext* pNewIoContext = pClientContext->GetNewIoContext();
	pNewIoContext->m_NetEvent = _IoContext::NET_RECV;
	pNewIoContext->m_Socket = pClientContext->m_Socket;
	
	if (PostRecv(pNewIoContext) == false)
	{
		pClientContext->RemoveContext(pNewIoContext);
		return false;
	}

	m_ClientContext.push_back(pClientContext);

	pIoContext->ResetBuffer();
	return PostAccept(pIoContext);
}

bool PostRecv(IoContext* pIoContext)
{
	DWORD dwFlags = 0;
	DWORD dwBytes = 0;
	WSABUF *pBuf = &pIoContext->m_wsabuf;
	OVERLAPPED* pOverLapped = &pIoContext->m_Overlapped;

	pIoContext->ResetBuffer();
	pIoContext->NET_RECV = IoContext::NET_RECV;

	int nBytesRecv = WSARecv(pIoContext->m_Socket, pBuf, 1, &dwBytes, &dwFlags, pOverLapped, NULL);
	if (nBytesRecv == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		fprintf(stderr, "Post recv failed!");
		return false;
	}

	return true;
}

bool ServerSocket::DoRecv(ClientContext* pSocketContext, IoContext* pIoContext)
{
	/*Show recv msg data, continue post next recv operator.*/
	//TODO...

	return PostRecv(pIoContext);
}

void SocketThread::Run()
{
	OVERLAPPED* pOverlapped = NULL;
	ClientContext* pClientSocket = NULL;
	DWORD dwBytes = 0;
	while (IsShutDown())
	{
		if(!GetQueuedCompletionStatus(
			m_ServerSocket.m_hIoCompletionPort, 
			&dwBytes, 
			(PULONG_PTR)&pClientSocket, 
			&pOverlapped, 
			INFINITE))
		{
			fprintf(stderr, "Get queued error, error code : %d", GetLastError());
			continue;
		}

		IoContext* pIoContext = CONTAINING_RECORD(pOverlapped, IoContext, m_Overlapped);
	}
}