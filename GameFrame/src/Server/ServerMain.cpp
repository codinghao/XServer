#include "Compact.h"
#include "MainService.h"
#include "UnitTest.h"
#include "NetService.h"

int main(int argc, char* argv[])
{
    //MemAllocTest();

    Service service;
    TcpServer server(&service, PeerAddr("0.0.0.0", 6020));

    NetService* ns[4];

    for (int i = 0; i < 4; i ++)
    {
        ns[i] = new NetService(service);
        ns[i]->Start();
    }

    for (int i = 0; i < 4; i ++)
    {
        ns[i]->Join();
    }

    return 0;
}