#include "HandlerThread.h"
#include "EventHandler.h"
#include "NetworkServiceImpl.h"

HandlerThread::HandlerThread(NetworkServiceImpl* pImpl)
    : m_pServiceImpl(pImpl)
{

}

HandlerThread::~HandlerThread()
{

}

void HandlerThread::Run()
{
    OVERLAPPED *pOverlapped = NULL;
    ConnectionSocket* pConn = NULL;
    DWORD dwBytes = 0;

    while(!IsShutDown())
    {
        /*bool bRet = GetQueuedCompletionStatus(
            m_pServiceImpl->m_hIoCompletionPort,
            &dwBytes,
            (ConnectionSocket*)&pConn,
            &pOverlapped,
            INFINITE);

        if (!bRet)
        {
            continue;
        }

        SocketContext* pContext = CONTAINING_RECORD(pOverlapped, SocketContextm, m_Overlapped);
        if ((dwBytes == 0) && (pContext->m_NetEvent == NET_RECV || pContext->m_NetEvent == NET_SEND))
        {
            m_pServiceImpl->DoClose(pConn);
            continue;
        }

        switch(pContext->m_NetEvent)  
        {
        case NET_ACCEPT:
            m_pServiceImpl->DoAccept(pContext);
            break;
        case NET_RECV:
            m_pServiceImpl->DoRecv(pConn, pContext);
            break;
        case NET_SEND:
            m_pServiceImpl->DoSend(pConn, pContext);
            break;
        default:
            break;
        }*/
    }
}