#include "OperationManager.h"
#include "OperationSet.h"

OperationManager::OperationManager()
{
    m_DestoryHandler = DestoryHandler(this, &OperationManager::OnDestory);
}

OperationManager::~OperationManager()
{
    std::for_each(m_OperationList.begin(), m_OperationList.end(), 
        std::bind1st(std::mem_fun1(&OperationManager::DeleteOperator), this));
    m_OperationList.clear();
}

Operation* OperationManager::CreateReadOperationFactory(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler)
{
    ReadOperation *readOp = new ReadOperation(_socket, _buffer, _handler, &m_DestoryHandler);
    m_OperationList.push_back(readOp);

    return readOp;
}

Operation* OperationManager::CreateWriteOperationFactory(Socket* _socket, const Buffer& _buffer, WriteHandler* _handler)
{
    WriteOperation *writeOp = new WriteOperation(_socket, _buffer, _handler, &m_DestoryHandler);
    m_OperationList.push_back(writeOp);

    return writeOp;
}

Operation* OperationManager::CreateAcceptOperationFactory(Socket* _socket, const Buffer& _buffer, AcceptHandler* _handler)
{
    AcceptOperation *acceptOp = new AcceptOperation(_socket, _buffer, _handler, &m_DestoryHandler);
    m_OperationList.push_back(acceptOp);

    return acceptOp;
}

void OperationManager::OnDestory(Operation* _operation, int _errorCode)
{
    std::remove_if(m_OperationList.begin(), m_OperationList.end(), 
        std::bind1st(std::equal_to<Operation*>(), _operation));
    DeleteOperator(_operation);
}

void OperationManager::DeleteOperator(Operation* _operation)
{
    delete _operation;
}