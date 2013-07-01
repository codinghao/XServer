#include "Compact.h"
#include "MainService.h"
#include "UnitTest.h"

int main(int argc, char* argv[])
{
    MemAllocTest();
    MainService ms;
    ms.Init();
    ms.Start();
	ms.Join();
    return 0;
}