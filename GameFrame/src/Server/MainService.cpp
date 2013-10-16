#include "MainService.h"
#include "GlobalData.h"

void MainService::Init() const
{
    g_ServerConfig.Init("ServerConfig.xml");
    g_ServerConfig.Print();
}

void MainService::Run()
{
	while(!IsShutDown())
    {
		//printf("Time : %u , Thread Running....\n", m_TimeService.CurrentTime());
        Sleep(m_TimeService.Delay());
        m_TimeService.Tick();
    }
}