#include "net.h"

namespace net {

tcpAccepter::tcpAccepter(transport *owner, Socket *socket,
			packetStreamer *streamer, serverAdapter *serverAdapter_ ) 
: ioComponent(owner,socket) {
	_streamer = streamer;
	_serverAdapter = serverAdapter_;	
}

bool tcpAccepter::init(bool isServer) {
	_socket->setSoBlocking(false);
	return ((serverSocket*) _socket)->listen();	
}

bool tcpAccepter::handleReadEvent() {
	Socket *socket;

	while ((socket = ((serverSocket*) _socket)->accept()) != NULL) {
		tcpComponent *component = new tcpComponent(_owner, socket, _streamer, _serverAdapter, false,false);
	
		if (!component->init(true)) {
			//SYS_LOG(ERROR,"init accept() is error");
			delete component;
			return true;	
		}	

		_owner->addComponent(component, true, false);	
	}	
	
	return true;	
}

void tcpAccepter::checkTimeOut(int64_t now) {
}

}//namespace net

