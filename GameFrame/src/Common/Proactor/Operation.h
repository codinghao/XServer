#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "Compact.h"

class Operation;

typedef void (*CompletionFunc)(Operation* _this, int _transferBytes, int _errorCode);
typedef void (*DestroyFunc)(Operation* _this, int _errorCode);

class Operation : public OVERLAPPED 
{
public:
    Operation(CompletionFunc _completionFunc, DestroyFunc _DestoryFunc)
        : m_CompletionFunc(_completionFunc)
        , m_DestoryFunc(_DestoryFunc)
    {
        Internal = 0;
        InternalHigh = 0;
        Offset = 0;
        OffsetHigh = 0;
        hEvent = 0;
    }

    ~Operation()
    {}

    void DoCompletion(int _transferBytes, int _errorCode) 
    {
        m_CompletionFunc(this, _transferBytes, _errorCode);
    }

    void DoDestory(int _errorCode)
    {
        m_DestoryFunc(this, _errorCode);
    }

private:
    CompletionFunc m_CompletionFunc;
    DestroyFunc m_DestoryFunc;
};

#endif