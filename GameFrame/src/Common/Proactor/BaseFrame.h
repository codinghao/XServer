#pragma once

enum EventType {
    ACCEPT = 0x01,
    READ   = 0x02,
    WRITE  = 0x03
};

typedef struct _Buffer {
    char* m_Buffer;
    int   m_Len;
}Buffer;

typedef struct _WinIO {
    OVERLAPPED m_Overlapped;
    Socket*    m_Socket;
    Buffer     m_Buffer;
}WinIO;

typedef Delegate<void (Socket*, char*, int)> AcceptEvent;
typedef Delegate<void (char*, int)> ReadEvent;
typedef Delegate<void (char*, int)> WriteEvent;

class Socket
{
public:
    Socket(Proactor* _proactor)
        : m_Proactor(_proactor)
    {

    }

    void AsyncRead(ReadEvent* _event, const Buffer& _buffer)
    {
        m_Proactor->PostRead(_event, _buffer);
    }

    void AsyncWrite(WriteEvent* _event, const Buffer& _buffer)
    {
        m_Proactor->PostWrite(_event, _buffer);
    }

private:
    Proactor* m_Proactor;
};

class Dispatcher
{
public:

};

class Proactor
{
public:
    bool Open();
    void Run();

    void PostAccept();
    void PostRead(ReadEvent* _event, const Buffer& _buffer);
    void PostWrite(WriteEvent* _event, const Buffer& _buffer);
};

class Acceptor
{
public:
    Acceptor(Proactor* _proactor)
        : m_Proactor(_proactor)
    {
    }

    void AsyncAccept();

private:
    Socket*   m_Socket;
    Proactor* m_Proactor;
};