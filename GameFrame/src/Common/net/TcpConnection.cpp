#include "TcpConnection.h"
#include "MemAlloc.h"

TcpConnection::TcpConnection(BreakenHandler& _breakenHandler)
    : m_BreakenHandler(_breakenHandler)
    , m_PendingReadCount(0)
    , m_PendingWriteCount(0)
{
    m_ReadHandler = ReadHandler(this, &TcpConnection::OnRead);
    m_WriteHandler = WriteHandler(this, &TcpConnection::OnWrite);
}

TcpConnection::~TcpConnection()
{
    Close();
}

void TcpConnection::OnRead(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    std::string data(_buffer->m_Buffer, _buffer->m_TransferLen);
    std::cout << data.c_str() << std::endl;

    MemAllocWithLockT.Dealloc(_buffer->m_Buffer, _buffer->m_MaxLen);
    InterlockedDecrement(&m_PendingReadCount);

    if (_errorCode != 0)
    {
        if (InterlockedCompareExchange(&m_PendingReadCount, -1, 0) == 0)
        {
            if (InterlockedCompareExchange(&m_PendingWriteCount, -1, 0) == 0)
                m_BreakenHandler(_socket);
            else
                InterlockedExchange(&m_PendingReadCount, 0);
        }
    }
    else
    {
        AsyncRecv();
    }
}

void TcpConnection::OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}

void TcpConnection::AsyncRecv()
{
    Buffer buffer = Buffer((char*)MemAllocWithLockT.Alloc(BUFFER_SIZE), BUFFER_SIZE);

    if (AsyncRead(&m_ReadHandler, buffer))
    {
        InterlockedIncrement(&m_PendingReadCount);
    }
    else 
    {
        MemAllocWithLockT.Dealloc(buffer.m_Buffer, buffer.m_MaxLen);
    }
}

void TcpConnection::AsyncSend()
{

}