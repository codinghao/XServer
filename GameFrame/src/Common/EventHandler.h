#ifndef _EVENT_HANDLE_H_
#define _EVENT_HANDLE_H_

#define MAX_EVENT_HANDLE 64

class SocketContext;
class TcpConnection;

class EventHandler
{
public:
    EventHandler() {}
    virtual ~EventHandler() {}

	virtual TcpConnection* OnAccept(const char* ip, const short port) = 0;
	virtual void OnSend() = 0;
	virtual void OnRecv(TcpConnection* pConn, SocketContext* pContext) = 0;
	virtual void OnClose() = 0;
};

#endif