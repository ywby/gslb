#ifndef NET_SERVERSOCKET_H_
#define NET_SERVERSOCKET_H_

namespace net {

class serverSocket : public Socket {
	
public:		
	serverSocket();
	~serverSocket();
	
	Socket *accept();

	bool listen();

private:
	int backLog;	
};

}//namespace net

#endif//NET_SERVERSOCKET_H_

