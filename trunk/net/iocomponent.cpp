#include "net.h"

namespace net {

ioComponent::ioComponent(transport *owner, Socket *socket){
	_owner = owner;
	_socket = socket;
	_socket->setIoComponent(this);
	_socketEvent = NULL;
	atomic_set(&_refcount,0);
	_state = NET_UNCONNECTED;
	_autoReconn = false;
	_prev = _next = NULL;
	_lastUseTime = sys::timeUtil::getTime();
	_inUsed = false;		
}

ioComponent::~ioComponent() {
	if (_socket) {
		_socket->close();
		
		delete _socket;
		_socket = NULL;	
	}	
}

transport *ioComponent::getOwner() {
	return _owner;	
}
	
}//namespace net 

