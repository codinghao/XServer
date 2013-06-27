#include "ServerConfig.h"
#include "TinyXml.h"

ServerConfig g_ServerConfig;

ServerConfig::ServerConfig()
    : m_LogLevel(0)
    , m_BindPort(0)
    , m_DataBasePort(0)
{
    ::memset(m_LogFile, 0, sizeof(m_LogFile));
    ::memset(m_BindIp, 0, sizeof(m_BindIp));
    ::memset(m_DataBaseIp, 0, sizeof(m_DataBaseIp));
    ::memset(m_ServerVersion, 0, sizeof(m_ServerVersion));
}

void ServerConfig::Init(const char* pConfigFile)
{
    assert(pConfigFile != NULL);

    TiXmlDocument xDoc;
    if (!xDoc.LoadFile(pConfigFile))
    {
        fprintf(stdout, "Load config file error!");
        assert(0);
    }

    TiXmlElement* pRoot = xDoc.RootElement();
    assert(pRoot != NULL);
    TiXmlElement* pConf = pRoot->FirstChildElement();
    assert(pConf != NULL);

    ::strcpy(m_BindIp,       pConf->Attribute("ip"));
    ::strcpy(m_DataBaseIp,   pConf->Attribute("dbip"));
    ::strcpy(m_ServerVersion,pConf->Attribute("version"));
    ::strcpy(m_LogFile,      pConf->Attribute("logfile"));

    pConf->QueryIntAttribute("port", &m_BindPort);
    pConf->QueryIntAttribute("dbport", &m_DataBasePort);
    pConf->QueryIntAttribute("loglevel", &m_LogLevel);
}

void ServerConfig::Print() const
{
    printf("|----------------------------------------------------------|\n");
    printf("| Version      : %s\n", m_ServerVersion);
    printf("| Bind IP      : %s\n", m_BindIp);
    printf("| Bind Port    : %d\n", m_BindPort);
    printf("| DataBase IP  : %s\n", m_DataBaseIp);
    printf("| DataBase Port: %d\n", m_DataBasePort);
    printf("| LogFile  Name: %s\n", m_LogFile);
    printf("| LogLevel     : %d\n", m_LogLevel);
    printf("|----------------------------------------------------------|\n");
}