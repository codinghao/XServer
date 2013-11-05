#ifndef _OPERATOR_H_
#define _OPERATOR_H_

#include "Socket.h"

class Operation;

typedef void (*InvokeFunc)(Operation* _this, int _transferBytes, int _errorCode);

class Operation : public OVERLAPPED 
{
public:
    Operation(InvokeFunc _invokeFunc)
        : m_InvokeFunc(_invokeFunc)
    {
        Internal = 0;
        InternalHigh = 0;
        Offset = 0;
        OffsetHigh = 0;
        hEvent = 0;
    }

    ~Operation()
    {}

    void DoInvoke(int _transferBytes, int _errorCode) 
    {
        m_InvokeFunc(this, _transferBytes, _errorCode);
    }

private:
    InvokeFunc m_InvokeFunc;
};

class ReadOperation : public Operation
{
public:
    ReadOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    ReadHandler* m_Handler;
};

class WriteOperation : public Operation
{
public:
    WriteOperation(Socket* _socket, const Buffer& _buffer, WriteHandler* _handler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    WriteHandler* m_Handler;
};

class AcceptOperation : public Operation
{
public:
    AcceptOperation(Socket* _socket, const Buffer& _buffer, AcceptHandler* _handler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    AcceptHandler* m_Handler;
};


#endif