#include "OperatorSet.h"
#include "Buffer.h"

ReadOperation::ReadOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler)
    : Operation(DoCompletion)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_Handler(_handler)
{
}

void ReadOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    ReadOperation* readOp = static_cast<ReadOperation*>(_this);

    readOp->m_Buffer.m_Len = _transferBytes;
    (*(readOp->m_Handler))(readOp->m_Socket, &(readOp->m_Buffer), _errorCode);
}

void ReadOperation::DoDestory(Operation* _this, int _errorCode)
{

}

WriteOperation::WriteOperation(Socket* _socket, const Buffer& _buffer, WriteHandler* _handler)
    : Operation(DoCompletion)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_Handler(_handler)
{}

void WriteOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    WriteOperation* writeOp = static_cast<WriteOperation*>(_this);

    writeOp->m_Buffer.m_Len = _transferBytes;
    (*(writeOp->m_Handler))(writeOp->m_Socket, &writeOp->m_Buffer, _errorCode);
}

void WriteOperation::DoDestory(Operation* _this, int _errorCode)
{

}

AcceptOperation::AcceptOperation(Socket* _socket, const Buffer& _buffer, AcceptHandler* _handler)
    : Operation(DoCompletion)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_Handler(_handler)
{
}

void AcceptOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    AcceptOperation* acceptOp = static_cast<AcceptOperation*>(_this);

    acceptOp->m_Buffer.m_Len = _transferBytes;
    (*(acceptOp->m_Handler))(acceptOp->m_Socket, &acceptOp->m_Buffer, _errorCode);
}

void AcceptOperation::DoDestory(Operation* _this, int _errorCode)
{

}