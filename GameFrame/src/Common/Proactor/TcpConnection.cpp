#include "TcpConnection.h"
#include "MemAlloc.h"

TcpConnection::TcpConnection()
{
    m_ReadHandler = ReadHandler(this, &TcpConnection::OnRead);
}

TcpConnection::~TcpConnection()
{

}

void TcpConnection::OnRead(Socket* _socket, Buffer* _buffer, int _errorCode)
{
    std::string data(_buffer->m_Buffer, _buffer->m_TransferLen);
    std::cout << "Recv Data:" << data.c_str() << std::endl;

    if (_errorCode == 0)
    {
        AsyncRead(&m_ReadHandler, Buffer((char*)MemAllocT.Alloc(BUFFER_SIZE), BUFFER_SIZE));
    }
}

void TcpConnection::OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}