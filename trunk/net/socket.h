#ifndef NET_SOCKET_H_
#define NET_SOCKET_H_

#include <string>

namespace net {

class Socket {
	
public:
	Socket();
	~Socket();

	bool setAddress(const char *address, const int port);
	
	bool connect();

	void close();
	
	void shutdown();

	void setUp(int socketHandle, struct sockaddr *hostAddress);

	int getSocketHandle();

	ioComponent *getIoComponent();

	void setIoComponent(ioComponent *ioc);	

	int write(const void *data,int len);

	int read(void *data, int len);

	bool setKeepAlive(bool on); 

	bool setReuseAddress(bool on); 

	bool setSoLinger(bool doLinger, int seconds);

	bool setTcpNoDelay(bool noDelay);
    
	bool setTcpQuickAck(bool quickAck);
    
	bool setIntOption(int option, int value);
    
    
	bool setSoBlocking(bool on);
    
	bool checkSocketHandle();
    
	int getSoError();
    
	std::string getAddr();
    
	uint64_t getId();
    
	uint64_t getPeerId();
    
	int getLocalPort();
    
	static int getLastError() {
        return errno;
	}

public :
	int _socketHandle;

protected:
	struct sockaddr_in _address;
	ioComponent *_iocomponent;
	static sys::threadMutex _dnsMutex; 
};

}//namespace net

#endif//NET_SOCKET_H_

