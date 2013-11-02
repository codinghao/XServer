#ifndef _OPERATOR_H_
#define _OPERATOR_H_

typedef void (InvokeFunc*)(Operation* _this, int _transferBytes, int _errorCode);

typedef Delegate<void (Buffer* /*_buffer*/, int /*_errorCode*/)> ReadHandler;
typedef Delegate<void (Buffer* /*_buffer*/, int /*_errorCode*/)> WriteHandler;
typedef Delegate<void (Socket* /*_socket*/, Buffer* /*_buffer*/, int /*_errorCode*/)> AcceptHandler;

class Operation : public OVERLAPPED 
{
public:
    Operation(InvokeFunc& _invokeFunc)
        : m_InvokeFunc(_invokeFunc)
    {}

    ~Operation();

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
    ReadOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler)
        : Operation(DoCompletion)
        , m_Socket(_socket)
        , m_Buffer(_buffer)
        , m_Handler(_handler)
    {
    }
    
    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
    {
        _this->m_Buffer.len = _transferBytes;
        (*(_this->m_Handler))(&m_Buffer, _errorCode);
    }
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    ReadHandler* m_Handler;
};

class WriteOperation : public Operation
{
public:
    WriteOperation(Socket* _socket, const Buffer& _buffer, ReadHandler* _handler)
        : Operation(DoCompletion)
        , m_Socket(_socket)
        , m_Buffer(_buffer)
        , m_Handler(_handler)
    {

    }

    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
    {
        _this->m_Buffer.len = _transferBytes;
        (*(_this->m_Handler))(&m_Buffer, _errorCode);
    }
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    WriteHandler* m_Handler;
};

class AcceptOperation : public Operation
{
public:
    AcceptOperation()
        : Operation(DoCompletion)
    {
    }

    static void DoCompletion(Operation* _this, int _transferBytes, int _errorCode)
    {
        _this->m_Buffer.len = _transferBytes;
        (*(_this->m_Handler))(m_Socket, &m_Buffer, _errorCode);
    }
public:
    Socket* m_Socket;
    Buffer  m_Buffer;
    AcceptHandler* m_Handler;
};


#endif