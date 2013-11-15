#include "ConnectionManager.h"
#include "TcpConnection.h"

ConnnectionManager::ConnnectionManager()
    : m_ConnCount(0)
{

}

ConnnectionManager::~ConnnectionManager()
{
    std::for_each(m_PendingConnList.begin(), m_PendingConnList.end(), 
        std::bind1st(std::mem_fun1(&ConnnectionManager::_DeleteTcpConnection), this));

    for (std::map<ulonglong, TcpConnection*>::iterator it = m_ConnMap.begin(); it != m_ConnMap.end(); ++ it)
    {
        _DeleteTcpConnection(it->second);
    }

    m_PendingConnList.clear();
    m_ConnMap.clear();
}

TcpConnection* ConnnectionManager::CreateTcpConnnectionFanctory()
{
    TcpConnection* pConn = new TcpConnection(BreakenHandler(this, &ConnnectionManager::RemoveTcpConnection));
    m_PendingConnList.push_back(pConn);
    
    return pConn;
}

void ConnnectionManager::AddTcpConnection(TcpConnection* _conn)
{
    assert(_conn != NULL);

    std::remove_if(m_PendingConnList.begin(), m_PendingConnList.end(), std::bind1st(std::equal_to<TcpConnection*>(), _conn));
    
    _conn->SetConnId(m_ConnCount);

    m_ConnMap.insert(std::make_pair(m_ConnCount++, _conn));
}

void ConnnectionManager::RemoveTcpConnection(Socket* _socket)
{
    assert(_socket != NULL);
    
    TcpConnection* pConn = static_cast<TcpConnection*>(_socket);
    
    std::cout << "Connection breaken : IP[" << pConn->GetIp() << "], PORT[" << pConn->GetPort() << "]" << std::endl;

    m_ConnMap.erase(pConn->GetConnId());
    _DeleteTcpConnection(pConn);
}

void ConnnectionManager::_DeleteTcpConnection(TcpConnection* _conn)
{
    delete _conn;
}