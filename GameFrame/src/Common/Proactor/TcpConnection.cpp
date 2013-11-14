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
    std::cout << "Recv Data:" << data.c_str() << std::endl;

    MemAllocT.Dealloc(_buffer->m_Buffer, _buffer->m_MaxLen);

    if (_errorCode != 0)
    {
        if (m_PendingReadCount == 0 && m_PendingWriteCount == 0)
            m_BreakenHandler(_socket);
    }
    else
    {
        AsyncRead(&m_ReadHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
    }
}

void TcpConnection::OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}