#include "net.h"

namespace net {

serverSocket::serverSocket() {
	backLog = 256;
}

serverSocket::~serverSocket() {
}

Socket *serverSocket::accept() {
	Socket *handleSocket = NULL;
	struct sockaddr_in addr;
	int len = sizeof(addr);	
	
	int fd = ::accept(_socketHandle,(struct sockaddr *) &addr,(socklen_t*) &len);			
	if (fd >=0 ) {
		handleSocket = new Socket();
		handleSocket->setUp(fd,(struct sockaddr *) &addr);	
	} else {
		int error = getLastError();
		if (error != EAGAIN){
			//sys.log	
		}	
	}

	return handleSocket;	
}

bool serverSocket::listen() {
	if (!checkSocketHandle()) {
		return false;	
	}

	setSoLinger(false,0);
	setReuseAddress(true);
	setIntOption(SO_KEEPALIVE,1);	
	setIntOption(SO_SNDBUF,640000);	
	setIntOption(SO_RCVBUF,640000);
	setTcpNoDelay(true);

	if(::bind(_socketHandle,(struct sockaddr *) &_address,sizeof(_address)) <0) {
		return false;	
	}

	if(::listen(_socketHandle,backLog) <0) {
		return false;	
	}

	return true;
}

}//namespace net

