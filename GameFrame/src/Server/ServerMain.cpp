#include "Compact.h"
#include "MainService.h"
#include "UnitTest.h"
#include "NetworkService.h"
#include "MsgHandler.h"

int main(int argc, char* argv[])
{
    //MemAllocTest();

    MsgHandler handler;
    NetworkService service(&handler);
    service.Init();
    service.Run("0.0.0.0", 6020);
    MainService ms;
    ms.Init();
    ms.Start();
	ms.Join();
    return 0;
}