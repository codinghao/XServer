#ifndef _LOGGER_H_
#define _LOGGER_H_ 

#include "Compact.h"

class Logger
{
public:
    static void Init(const char* path, int level = 0);
    void Func(const char* subsys, const char* file, int line, const char* func, const char* format, ...);    
    void Close();
public:    
    static int   m_LogLevel;
    static FILE* m_LogFile;
};

#endif  //Logger.h