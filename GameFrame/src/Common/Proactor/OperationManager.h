#ifndef _OPERATION_MANAGER_H_
#define _OPERATION_MANAGER_H_

#include "Compact.h"
#include "EventHandler.h"

class OperationManager
{
public:
    OperationManager();
    ~OperationManager();

    Operation* CreateReadOperationFactory(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler);
    Operation* CreateWriteOperationFactory(Socket* _socket, const Buffer& _buffer, WriteHandler* _handler);
    Operation* CreateAcceptOperationFactory(Socket* _socket, const Buffer& _buffer, AcceptHandler* _handler);

    void OnDestory(Operation* _operation, int _errorCode);
    void DeleteOperator(Operation* _operation);

private:
    std::list<Operation*> m_OperationList;
    DestoryHandler m_DestoryHandler;
};

static OperationManager gs_OperationManager;

#endif