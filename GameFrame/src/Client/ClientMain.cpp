#include "Connection.h"

void main(int argc, char* argv[])
{
    Connection conn("127.0.0.1", 6020);
    conn.Start();

    char buf[512] = {0};
    while(true)
    {
        ::memset(buf, 0, sizeof(buf));
        std::cin >> buf;
        if (!conn.Send(buf, strlen(buf)))
            break;
    }

    conn.Join();
}