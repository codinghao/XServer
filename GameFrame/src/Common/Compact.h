#ifndef _COMPACT_H_
#define _COMPACT_H_

#if defined(_WIN32)
    #pragma warning ( disable : 4786 )   //编译时模板中标识符过长
    #pragma warning ( disable : 4996 )   //编译提示C中函数不安全
    #pragma warning ( disable : 4345 )   //POD类型构造时使用默认初始化
    #include <WinSock2.h>
	#include <Windows.h>
	#include <crtdbg.h>
    #pragma comment ( lib, "winmm.lib" )
#endif

#include <stdio.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <stdarg.h>
#include <memory.h>
#include <assert.h>

typedef unsigned char	UCHAR;		
typedef char			CHAR;		
typedef unsigned int	UINT;		
typedef int				INT;		
typedef unsigned short	USHORT;		
typedef short			SHORT;		
typedef unsigned long	ULONG;		
typedef long			LONG;		
typedef float			FLOAT;		

typedef UCHAR			uchar;
typedef USHORT			ushort;
typedef UINT			uint;
typedef ULONG			ulong;
typedef INT				BOOL;
typedef UCHAR			BYTE;

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 0
#endif

#ifndef VOID
    #define VOID void
#endif

#endif