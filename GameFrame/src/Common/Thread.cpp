#include "Thread.h"

#if defined(_WIN32)
#include <process.h>
uint _stdcall ThreadProc(void* pParam)
{
    Thread* pThread = (Thread*)pParam;
    if (pThread == NULL)
        return 0;
    pThread->SetStatus(Thread::RUNNING);
    pThread->Run();
    pThread->SetStatus(Thread::EXIT);
    pThread->Exit();
    return 0;
}
#endif

Thread::Thread()
    : m_Id(0)
    , m_Status(Thread::READY)
    , m_ShutDown(0)
#if defined(_WIN32)
    , m_Handle(NULL)
#endif
{

}

Thread::~Thread()
{
    Stop();
    Join();
}

void Thread::Start()
{
    if (m_Status != Thread::READY)
        return ;
#if defined(_WIN32)
    m_Handle = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, this, 0, &m_Id);
#endif
}

void Thread::Join()
{
    if (m_Id == 0)
        return ;
#if defined(_WIN32)
    int rv = ::WaitForSingleObject(m_Handle, INFINITE);
	if (rv == WAIT_OBJECT_0 || rv == WAIT_ABANDONED)
	{

	}
    Exit();
#endif
}

void Thread::Stop()
{
#if defined(_WIN32)
    ::InterlockedIncrement(&m_ShutDown);
#endif
}

void Thread::Exit()
{
#if defined(_WIN32)
    CloseHandle(m_Handle);
    m_Handle = NULL;
#endif
    m_Id = 0;
}

bool Thread::IsShutDown() 
{ 
    return m_ShutDown > 0;
}

Thread::ThreadStatus Thread::GetStatus() 
{ 
    return m_Status; 
}

void Thread::SetStatus(ThreadStatus status) 
{ 
    m_Status = status; 
}