#include "net.h"

namespace net {

epollSocketEvent::epollSocketEvent() {
	_iepfd = epoll_create(MAX_SOCKET_EVENTS);	
}
	
epollSocketEvent::~epollSocketEvent() {
	close(_iepfd);	
}

bool epollSocketEvent::addEvent(Socket *socket, bool enableRead, bool enableWrite) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = socket->getIoComponent();
	ev.events = 0;
	
	if (enableRead) {
		ev.events |= EPOLLIN;	
	}

	if (enableWrite) {
		ev.events |= EPOLLOUT;	
	}

	bool ret = (epoll_ctl(_iepfd,EPOLL_CTL_ADD,socket->getSocketHandle(),&ev) == 0);

	return ret;	
}

bool epollSocketEvent::setEvent(Socket *socket, bool enableRead, bool enableWrite) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = socket->getIoComponent();
	ev.events = 0;
	
	if (enableRead) {
		ev.events |= EPOLLIN;	
	}
	
	if (enableWrite) {
		ev.events |= EPOLLOUT;	
	}	

	bool ret = (epoll_ctl(_iepfd, EPOLL_CTL_MOD, socket->getSocketHandle(), &ev) == 0);
	
	return ret;	
}

bool epollSocketEvent::removeEvent(Socket *socket) {
	struct epoll_event ev;
	memset(&ev, 0, sizeof(ev));
	ev.data.ptr = socket->getIoComponent();
	ev.events = 0;

	bool ret = (epoll_ctl(_iepfd, EPOLL_CTL_DEL, socket->getSocketHandle(), &ev) == 0);
		
	return ret;	
}

int epollSocketEvent::getEvents(int timeout, ioEvent *ioevents, int cnt) {
	struct epoll_event events[MAX_SOCKET_EVENTS];

	if (cnt > MAX_SOCKET_EVENTS) {
		cnt = MAX_SOCKET_EVENTS;
	}

	int res = epoll_wait(_iepfd, events, cnt , timeout);
	if (res > 0) {
		memset(ioevents, 0, sizeof(ioEvent) * res);
	}

	for (int i = 0; i < res; i++) {
		ioevents[i]._ioc = (ioComponent*)events[i].data.ptr;
		if (events[i].events & (EPOLLERR | EPOLLHUP)) {
			ioevents[i]._errorEvent  = true;
		}
		if ((events[i].events & EPOLLIN) != 0) {
			ioevents[i]._readEvent = true;
		}
		if ((events[i].events & EPOLLOUT) != 0) {
			ioevents[i]._writeEvent = true;
		}
	}

	return res;
}	

}//namespace net

