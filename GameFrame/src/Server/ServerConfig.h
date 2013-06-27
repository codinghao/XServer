#ifndef _SERVER_CONFIG_H_
#define _SERVER_CONFIG_H_

#include "Compact.h"

#define MAX_FILE_PATH 256
#define MAX_IP_LENGTH 16
#define MAX_VERSION_LENGTH 8

class ServerConfig
{
public:
    ServerConfig();
    ~ServerConfig(){}

    void Init(const char* pConfigFile);
    void Print() const;

public:
    int    m_LogLevel;    
    char   m_LogFile[MAX_FILE_PATH];
    char   m_BindIp[MAX_IP_LENGTH];
    int    m_BindPort;
    char   m_DataBaseIp[MAX_IP_LENGTH];
    int    m_DataBasePort;
    char   m_ServerVersion[MAX_VERSION_LENGTH];
};

extern ServerConfig g_ServerConfig;

#endif