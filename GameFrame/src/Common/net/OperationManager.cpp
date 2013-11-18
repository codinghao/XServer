#include "OperationManager.h"
#include "OperationSet.h"

OperationManager::OperationManager()
{
    m_DestoryHandler = DestoryHandler(this, &OperationManager::OnDestory);
}

OperationManager::~OperationManager()
{

}

Operation* OperationManager::CreateReadOperationFactory(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler)
{
    ReadOperation *readOp = new ReadOperation(_socket, _buffer, _handler, &m_DestoryHandler);

    return readOp;
}

Operation* OperationManager::CreateWriteOperationFactory(Socket* _socket, const Buffer& _buffer, WriteHandler* _handler)
{
    WriteOperation *writeOp = new WriteOperation(_socket, _buffer, _handler, &m_DestoryHandler);
    return writeOp;
}

Operation* OperationManager::CreateAcceptOperationFactory(Socket* _socket, const Buffer& _buffer, AcceptHandler* _handler)
{
    AcceptOperation *acceptOp = new AcceptOperation(_socket, _buffer, _handler, &m_DestoryHandler);

    return acceptOp;
}

void OperationManager::OnDestory(Operation* _operation, int _errorCode)
{
    delete _operation;
}