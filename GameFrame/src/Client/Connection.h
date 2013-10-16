#pragma once

#include <Compact.h>
#include <Thread.h>

#pragma comment(lib, "ws2_32.lib")

class Connection : public Thread
{
public:
    Connection(const char* _ip, ushort _port)
        : m_Port(_port)
    {
        strcpy(m_Ip, _ip);

        WSADATA wsaData;
        int ret= WSAStartup(MAKEWORD(2,2), &wsaData);
        if (NO_ERROR != ret)
        {
            fprintf(stderr, "Failed call WSAStartup function.");
            return ;
        }

        m_Scoket = socket(AF_INET, SOCK_STREAM, 0);
        if (m_Scoket == INVALID_SOCKET)
            std::cerr << "create socket faild!" << std::endl;
    }

    void Run()
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_addr.s_addr = ::inet_addr(m_Ip);
        addr.sin_port = ntohs(m_Port);
        ::connect(m_Scoket, (const sockaddr*)&addr, sizeof(sockaddr));

        while(true)
        {
            ::memset(m_Buf, 0, sizeof(m_Buf));
            int rv = ::recv(m_Scoket, m_Buf, sizeof(m_Buf), 0);
            if (rv == SOCKET_ERROR)
            {
                std::cerr << "recv error : " << WSAGetLastError() << std::endl;
                break;
            }
            
            if (rv > 0)
                std::cout << "recv content : " << m_Buf << std::endl;
        }
    }

    bool Send(const char* _buf, int _len)
    {
        if (::send(m_Scoket, _buf, _len, 0) == SOCKET_ERROR)
        {
            std::cerr << "send error : " << WSAGetLastError() << std::endl;
            return false
        }

        return true;
    }

private:
    char m_Ip[16];
    ushort m_Port;
    SOCKET m_Scoket;
    char m_Buf[512];
};