#ifndef NET_SOCKETEVENT_H_
#define NET_SOCKETEVENT_H_

namespace net {

#define MAX_SOCKET_EVENTS 256

class ioEvent {

public :
	bool _readEvent;
	bool _writeEvent;
	bool _errorEvent;
	ioComponent *_ioc;
};

class socketEvent {
	
public:
	socketEvent();
	~socketEvent();
	
	virtual bool addEvent(Socket *socket_, bool enableRead, bool enableWrite) = 0;

	virtual bool setEvent(Socket *socket_, bool enableRead, bool enableWrite) = 0;

	virtual bool removeEvent(Socket *socket_) = 0;

	virtual int getEvents(int timeout, ioEvent *events, int cnt) = 0 ;	
};

}//namespace net

#endif//NET_SOCKETEVENT_H_

