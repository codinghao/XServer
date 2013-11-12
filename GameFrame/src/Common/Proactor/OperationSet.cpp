#include "OperationSet.h"
#include "Socket.h"
#include "Buffer.h"

/***********************************ReadOperation************************************/

ReadOperation::ReadOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _readHandler, DestoryHandler* _destoryHandler)
    : Operation(DoCompletion, DoDestory)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_ReadHandler(_readHandler)
    , m_DestoryHandler(_destoryHandler)
{
}

void ReadOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    ReadOperation* readOp = static_cast<ReadOperation*>(_this);

    readOp->m_Buffer.m_TransferLen = _transferBytes;
    (*(readOp->m_ReadHandler))(readOp->m_Socket, &(readOp->m_Buffer), _errorCode);
}

void ReadOperation::DoDestory(Operation* _this, int _errorCode)
{
    ReadOperation* readOp = static_cast<ReadOperation*>(_this);
    (*(readOp->m_DestoryHandler))(_this, _errorCode);
}

/***********************************WriteOperation************************************/

WriteOperation::WriteOperation(Socket* _socket, const Buffer& _buffer, WriteHandler* _writeHandler, DestoryHandler* _destoryHandler)
    : Operation(DoCompletion, DoDestory)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_WriteHandler(_writeHandler)
    , m_DestoryHandler(_destoryHandler)
{}

void WriteOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    WriteOperation* writeOp = static_cast<WriteOperation*>(_this);

    writeOp->m_Buffer.m_TransferLen = _transferBytes;
    (*(writeOp->m_WriteHandler))(writeOp->m_Socket, &writeOp->m_Buffer, _errorCode);
}

void WriteOperation::DoDestory(Operation* _this, int _errorCode)
{
    WriteOperation* readOp = static_cast<WriteOperation*>(_this);
    (*(readOp->m_DestoryHandler))(_this, _errorCode);
}

/***********************************AcceptOperation************************************/

AcceptOperation::AcceptOperation(Socket* _socket, const Buffer& _buffer, AcceptHandler* _acceptHandler, DestoryHandler* _destoryHandler)
    : Operation(DoCompletion, DoDestory)
    , m_Socket(_socket)
    , m_Buffer(_buffer)
    , m_AcceptHandler(_acceptHandler)
    , m_DestoryHandler(_destoryHandler)
{
}

void AcceptOperation::DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
{
    AcceptOperation* acceptOp = static_cast<AcceptOperation*>(_this);

    acceptOp->m_Buffer.m_TransferLen = _transferBytes;
    (*(acceptOp->m_AcceptHandler))(acceptOp->m_Socket, &acceptOp->m_Buffer, _errorCode);
}

void AcceptOperation::DoDestory(Operation* _this, int _errorCode)
{
    AcceptOperation* readOp = static_cast<AcceptOperation*>(_this);
    (*(readOp->m_DestoryHandler))(_this, _errorCode);
}