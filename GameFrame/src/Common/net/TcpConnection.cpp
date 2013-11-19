#include "TcpConnection.h"
#include "MemAlloc.h"

TcpConnection::TcpConnection(ReadHandler& _readHandler, WriteHandler& _writeHandler)
    : m_ReadHandler(_readHandler)
    , m_WriteHandler(_writeHandler)
{

}

TcpConnection::~TcpConnection()
{
    Close();
}

void TcpConnection::OnRead(Buffer* _buffer)
{
    std::string data(_buffer->m_Buffer, _buffer->m_TransferLen);
    std::cout << data.c_str() << std::endl;
    MemAllocWithLockT.Dealloc(_buffer->m_Buffer, _buffer->m_MaxLen);
    AsyncRecv();
}

void TcpConnection::OnWrite(Buffer* _buffer)
{

}

void TcpConnection::AsyncRecv()
{
    Buffer buffer = Buffer((char*)MemAllocWithLockT.Alloc(BUFFER_SIZE), BUFFER_SIZE);

    if (!AsyncRead(&m_ReadHandler, buffer))
        MemAllocWithLockT.Dealloc(buffer.m_Buffer, buffer.m_MaxLen);
}

void TcpConnection::AsyncSend()
{

}