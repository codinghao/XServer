#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

#include "Compact.h"

class TcpConnection;

class ConnnectionManager
{
public:
    ConnnectionManager();
    ~ConnnectionManager();

    TcpConnection* CreateTcpConnnectionFanctory();    

    void AddTcpConnection(TcpConnection* _conn);
    void RemoveTcpConnection(TcpConnection* _conn);
private:
    void _DeleteTcpConnection(TcpConnection* _conn);

private:
    ulonglong m_ConnCount;
    std::map<ulonglong, TcpConnection*> m_ConnMap;
    std::list<TcpConnection*> m_PendingConnList;
};

#endif