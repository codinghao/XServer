#include "Mutex.h"

 Mutex::Mutex()
 {
#if defined(_WIN32)
     ::InitializeCriticalSection(&m_Id);
#endif
 }
 
 Mutex::~Mutex()
 {
#if defined(_WIN32)
     ::DeleteCriticalSection(&m_Id);
#endif
 }
 
 void Mutex::Lock()
 {
#if defined(_WIN32)
     ::EnterCriticalSection(&m_Id);
#endif
 }
 
 void Mutex::UnLock()
 {
#if defined(_WIN32)
     ::LeaveCriticalSection(&m_Id);
#endif
 }

AutoLock::AutoLock(Mutex& mtx)
    : m_Mutex(mtx)
{
    m_Mutex.Lock();
}

AutoLock::~AutoLock()
{
    m_Mutex.UnLock();
}