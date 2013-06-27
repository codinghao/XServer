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
    while(!IsShutDown())
    {

    }
}