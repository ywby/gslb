#ifndef NET_TRANSPORT_H_
#define NET_TRANSPORT_H_

namespace net {

class transport : public sys::runnable {

public:
	transport();
	
    ~transport();
	
	bool start();

	bool stop();
	
	bool wait();

	void run(sys::cThread *thread ,void *arg);

	ioComponent *listen(const char *spec , packetStreamer *streamer, serverAdapter *_serverAdapter);
	
	connection *connect(const char *spec, packetStreamer *streamer, bool autoReconn = false, bool appTcp = false);

	bool disconnect(connection *conn);
    
	void addComponent(ioComponent *ioc, bool readOn, bool writeOn);
    
	void removeComponent(ioComponent *ioc);
    
	bool* getStop();

private:    
	int splitAddr(char *src, char **args, int cnt);

	void eventLoop(socketEvent *socketEvent);

	void timeoutLoop();

	void destroy();

private:
	epollSocketEvent _socketEvent;

	sys::cThread _readWriteThread;
	sys::cThread _timeoutThread;

	bool _stop;

	ioComponent *_delListHead, *_delListTail;
	ioComponent *_iocListHead, *_iocListTail;

	bool _iocListChanged;
	int _iocListCount;

	sys::threadMutex _iocMutex;
	//std::map<int, connection *> socketConnMap; 
};	

}//namespace net

#endif//NET_TRANSPORT_H_

