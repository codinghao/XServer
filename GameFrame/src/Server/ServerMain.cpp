#include "Compact.h"
#include "MainService.h"
#include "UnitTest.h"

#include "./net/Service.h"
#include "./net/TcpServer.h"

int main(int argc, char* argv[])
{
    //MemAllocTest();

    Service service;
    TcpServer server(&service, PeerAddr("0.0.0.0", 6020));

    service.ServiceRun();

    return 0;
}