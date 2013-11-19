#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

#include "Compact.h"
#include "Mutex.h"
#include "EventHandler.h"

class Socket;
class TcpConnection;

class ConnnectionManager
{
public:
    ConnnectionManager();
    ~ConnnectionManager();

    TcpConnection* CreateTcpConnnectionFanctory(ReadHandler& _readHandler, WriteHandler& _writeHandler);    

    void AddTcpConnection(TcpConnection* _conn);
    bool FindAndRemoveConnection(TcpConnection* _conn);
    void DeleteTcpConnection(TcpConnection* _conn);

private:
    long volatile m_ConnCount;
    std::set<TcpConnection*> m_ConnectedSet;
    std::list<TcpConnection*> m_PendingConnList;
    Mutex m_PendingConnListMutex;
    Mutex m_ConnMapMutex;
};

#endif