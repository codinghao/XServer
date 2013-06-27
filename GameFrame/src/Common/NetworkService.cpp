#include "NetworkService.h"
#include "EventHandler.h"
#include "NetworkServiceImpl.h"

NetworkService::NetworkService(EventHandler* pHandler)
    : m_pImpl(new NetworkServiceImpl(pHandler))
{

}

NetworkService::~NetworkService()
{
    m_pImpl->Stop();
    delete m_pImpl;
}

bool NetworkService::Init()
{
    return m_pImpl->Init();
}

void NetworkService::Run(const char* ip, const short port)
{
    m_pImpl->Run(ip, port);
}

void NetworkService::Stop()
{
    m_pImpl->Stop();
}
