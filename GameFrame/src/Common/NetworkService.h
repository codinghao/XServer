#ifndef _NETWORK_SERVICE_H_
#define _NETWORK_SERVICE_H_

class EventHandler;
class NetworkServiceImpl;

class NetworkService
{
public:
	NetworkService(EventHandler* pHandler);
	~NetworkService();
	bool Init();
	void Run(const char* ip, const short port);
	void Stop();
private:
	NetworkServiceImpl* m_pImpl;
};

#endif