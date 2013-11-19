#include "ConnectionManager.h"
#include "TcpConnection.h"
#include "Atomic.h"

ConnnectionManager::ConnnectionManager()
    : m_ConnCount(0)
{

}

ConnnectionManager::~ConnnectionManager()
{
    std::for_each(m_PendingConnList.begin(), m_PendingConnList.end(), 
        std::bind1st(std::mem_fun1(&ConnnectionManager::DeleteTcpConnection), this));

    std::for_each(m_ConnectedSet.begin(), m_ConnectedSet.end(),
        std::bind1st(std::mem_fun1(&ConnnectionManager::DeleteTcpConnection), this));

    m_PendingConnList.clear();
    m_ConnectedSet.clear();
}

TcpConnection* ConnnectionManager::CreateTcpConnnectionFanctory(ReadHandler& _readHandler, WriteHandler& _writeHandler)
{
    TcpConnection* pConn = new TcpConnection(_readHandler, _writeHandler);

    m_PendingConnListMutex.Lock();
    m_PendingConnList.push_back(pConn);
    m_PendingConnListMutex.UnLock();
    
    return pConn;
}

void ConnnectionManager::AddTcpConnection(TcpConnection* _conn)
{
    assert(_conn != NULL);

    m_PendingConnListMutex.Lock();
    std::remove_if(m_PendingConnList.begin(), m_PendingConnList.end(), std::bind1st(std::equal_to<TcpConnection*>(), _conn));
    m_PendingConnListMutex.UnLock();
    
    _conn->SetConnId(AtomicCountInc(m_ConnCount));

    m_ConnMapMutex.Lock();
    m_ConnectedSet.insert(_conn);
    m_ConnMapMutex.UnLock();
}

bool ConnnectionManager::FindAndRemoveConnection(TcpConnection* _conn)
{
    m_ConnMapMutex.Lock();
    std::set<TcpConnection*>::iterator it = m_ConnectedSet.find(_conn);
    if (it == m_ConnectedSet.end())
    {
        m_ConnMapMutex.UnLock();
        return false;
    }
    m_ConnectedSet.erase(_conn);
    m_ConnMapMutex.UnLock();

    return true;
}

void ConnnectionManager::DeleteTcpConnection(TcpConnection* _conn)
{
    delete _conn;
}