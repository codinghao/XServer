#ifndef _CONNECTION_MANAGER_H_
#define _CONNECTION_MANAGER_H_

class ConnnectionManager
{
public:
    ConnnectionManager()
        : m_ConnCount(0)
    {}

    TcpConnection* CreateTcpConnnection();

private:
    uint m_ConnCount;
    std::map<uint, TcpConnection*> m_ConnMap;
    ObjectPoolWithLock<TcpConnection> m_ConnPool;
};

#endif