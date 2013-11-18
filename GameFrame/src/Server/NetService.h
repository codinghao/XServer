#ifndef _NET_SERVICE_H_
#define _NET_SERVICE_H_

#include "./net/SocketEnv.h"
#include "./net/Service.h"
#include "./net/TcpServer.h"


class NetService : public Thread
{
public:
    NetService(Service& _service)
        : m_Service(_service)
    {

    }

    void Run()
    {
        m_Service.ServiceRun();
    }
private:
    Service& m_Service;
};


#endif