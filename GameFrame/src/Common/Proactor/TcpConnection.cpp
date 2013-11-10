#include "TcpConnection.h"

TcpConnection::TcpConnection()
{
    m_ReadHandler = ReadHandler(this, &TcpConnection::OnRead);
}

void TcpConnection::OnRead(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}

void TcpConnection::OnWrite(Socket* _socket, Buffer* _buffer, int _errorCode)
{

}