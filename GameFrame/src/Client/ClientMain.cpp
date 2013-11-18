#include "Connection.h"

void SocketEnv()
{
    WSADATA wsaData;
    int ret= WSAStartup(MAKEWORD(2,2), &wsaData);
    if (NO_ERROR != ret)
    {
        fprintf(stderr, "Failed call WSAStartup function.");
        return ;
    }
}

void main(int argc, char* argv[])
{
    SocketEnv();

    std::list<Connection*> connList;

    for (int i = 0; i < 1000; i ++)
    {
        Connection* pConn = new Connection("127.0.0.1", 6020);
        pConn->Start();
        connList.push_back(pConn);

        //if ( i % 100 == 0)
        //    Sleep(2000);
    }

    char buf[512] = {0};
    while(true)
    {
        ::memset(buf, 0, sizeof(buf));
        std::cin >> buf;

        for (std::list<Connection*>::iterator it = connList.begin(); it != connList.end(); ++ it)
        {
            if (!(*it)->Send(buf, strlen(buf)))
                continue;
        }
    }
}