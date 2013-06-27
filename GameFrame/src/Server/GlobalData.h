#ifndef _GLOBAL_DATA_H_
#define _GLOBAL_DATA_H_

#include "ServerConfig.h"
#include "Logger.h"

/*Server config define*/
#define SERVERIP   (g_ServerConfig.m_BindIp)
#define SERVERPORT (g_ServerConfig.m_BindPort)
#define DBIP       (g_ServerConfig.m_DataBaseIp)
#define DBPORT     (g_ServerConfig.m_DataBasePort)

#endif 