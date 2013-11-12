#ifndef _OPERATION_SET_H_
#define _OPERATION_SET_H_

#include "Operation.h"
#include "EventHandler.h"

class ReadOperation : public Operation
{
public:
    ReadOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _readHandler, DestoryHandler* _destoryHandler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
    static void DoDestory(Operation* _this, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    ReadHandler* m_ReadHandler;
    DestoryHandler* m_DestoryHandler;
};

class WriteOperation : public Operation
{
public:
    WriteOperation(Socket* _socket, const Buffer& _buffer, WriteHandler* _writeHandler, DestoryHandler* _destoryHandler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
    static void DoDestory(Operation* _this, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    WriteHandler* m_WriteHandler;
    DestoryHandler* m_DestoryHandler;
};

class AcceptOperation : public Operation
{
public:
    AcceptOperation(Socket* _socket, const Buffer& _buffer, AcceptHandler* _acceptHandler, DestoryHandler* _destoryHandler);
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode);
    static void DoDestory(Operation* _this, int _errorCode);
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    AcceptHandler* m_AcceptHandler;
    DestoryHandler* m_DestoryHandler;
};

#endif