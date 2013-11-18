#ifndef _SOCKET_ENV_H_
#define _SOCKET_ENV_H_

#include "NoneCopyable.h"

class SocketEnv : public NoneCopyable
{
public:
    struct EnvInit
    {
        EnvInit()
        {
            WSADATA wsaData;
            int result = WSAStartup(MAKEWORD(2,2), &wsaData);
            if (result != NO_ERROR)
            {
                fprintf(stderr, "Failed WSAStartup call. Error code : %d\n", WSAGetLastError());
                assert(0);
            }
        }

        ~EnvInit()
        {
            WSACleanup();
        }
    };
private:
    static EnvInit m_Instance;
};

SocketEnv::EnvInit SocketEnv::m_Instance;

#endif