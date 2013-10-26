#pragma once

enum EventType
{
    ACCEPT = 0x01,
    READ   = 0x02,
    WRITE  = 0x03
};

typedef struct _Buffer 
{
    char* m_Buffer;
    int   m_Len;
}Buffer;

typedef struct _WinIO
{
    OVERLAPPED m_Overlapped;
    Socket*    m_Socket;
    Buffer     m_Buffer;
    EventType  m_OpType;
}WinIO;

typedef void (*AcceptFunc)(Socket* _socket, char* _buffer, int _bytes);
typedef void (*ReadFunc)(char* _buffer, int _bytes);
typedef void (*WriteFunc)(char* _buffer, int _bytes);

class Socket
{
public:
    Socket(Proactor& _proactor)
        : m_Proactor(_proactor)
    {

    }

    void AsyncRead(ReadFunc* _readFunc, const Buffer& _buffer)
    {
        m_Proactor.RegisterRead(this, _readFunc);
        m_Proactor.PostRead();
    }

    void AsyncWrite(WriteFunc* _writeFunc, char* _buffer, int _len)
    {

    }

private:
    Proactor& m_Proactor;
};

class Dispatcher
{
public:

};

class Proactor
{
public:
    bool Open();

    void RegisterAccept(AcceptFunc _acceptFunc);
    void RegisterRead(Socket* _socket, ReadFunc _readFunc);
    void RegisterWrite(Socket* _socket, WriteFunc _writeFunc);

    void Run();

    void PostAccept();
    void PostRead(Socket* _socket);
    void PostWrite();

private:
    AcceptFunc m_Accepted;
    ReadFunc m_Read;
    WriteFunc m_Write;
};

class Acceptor
{
public:
    Acceptor(Proactor& _proactor)
        : m_Proactor(_proactor)
    {
    }

    void AsyncAccept();

private:
    Proactor& m_Proactor;
};