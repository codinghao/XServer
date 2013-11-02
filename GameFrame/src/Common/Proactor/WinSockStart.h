#ifndef _WIN_SOCK_START_H_
#define _WIN_SOCK_START_H_

class WinSockStart : public NoneCopyable
{
public:
    struct StartUp
    {
        StartUp()
        {
            WSADATA wsaData;
            int result = WSAStartup(MAKEWORD(2,2), &wsaData);
            if (result != NO_ERROR)
            {
                fprintf(stderr, "Failed WSAStartup call. Error code : %d\n", WSAGetLastError());
                assert(0);
            }
        }

        ~StartUp()
        {
            WSACleanup();
        }
    };
private:
    static StartUp m_Instance;
};

WinSockStart::StartUp WinSockStart::m_Instance;

#endif