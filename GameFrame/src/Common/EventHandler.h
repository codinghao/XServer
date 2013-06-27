#ifndef _EVENT_HANDLE_H_
#define _EVENT_HANDLE_H_

#define MAX_EVENT_HANDLE 64

class SocketContext;
class ConnectionSocket;

class EventHandler
{
public:
	EventHandler();
	virtual ~EventHandler();

	virtual ConnectionSocket* OnAccept(const char* ip, const short port);
	virtual void OnSend() = 0;
	virtual void OnRecv(ConnectionSocket* pConn, SocketContext* pContext) = 0;
	virtual void OnClose() = 0;
private:

};

#endif