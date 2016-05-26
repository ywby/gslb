#ifndef NET_EPOLLSOCKETEVENT_H_
#define NET_EPOLLSOCKETEVENT_H_

namespace net {

class epollSocketEvent : public socketEvent {
	
public :
	epollSocketEvent();

	~epollSocketEvent();

	bool addEvent(Socket *socket, bool enableRead, bool enableWrite);		
	
	bool setEvent(Socket *socket, bool enableRead, bool enableWrite);

	bool removeEvent(Socket *socket);

	int getEvents(int timeout, ioEvent *events, int cnt);	

private:
	int _iepfd;
};

}//namespace net

#endif//NET_EPOLLSOCKETEVENT_H_

