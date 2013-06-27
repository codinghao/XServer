#include "Logger.h"

int Logger::m_LogLevel = 0;
FILE* Logger::m_LogFile = stdout;

void Logger::Init(const char* path, int level)
{
	m_LogLevel = level;

	if (path != NULL) 
	{
		m_LogFile = fopen(path, "w+");
		if (m_LogFile == NULL) 
		{
			fprintf(stderr, "Open log file failed!\n");		
			m_LogFile = stdout;
		}
	}
}

void Logger::Func(const char* subsys, const char* file, int line, const char* func, const char* format, ...)
{
	va_list arg_ptr;
	va_start(arg_ptr, format);
	fprintf(m_LogFile, "%s::%s line:%d %s ", subsys, file, line, func);
	fprintf(m_LogFile, format, arg_ptr);
	va_end(arg_ptr);
}

void Logger::Close()
{
	if (m_LogFile != NULL && m_LogFile != stdout)
	{
		fclose(m_LogFile);
	}
}