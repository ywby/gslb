#include "net.h"

namespace net {

sys::threadMutex Socket::_dnsMutex;

Socket::Socket() {
	_socketHandle = -1;	
}

Socket::~Socket() {
	close();	
}

bool Socket::setAddress(const char *address, const int port) {
	memset(static_cast<void *>(&_address),0,sizeof(_address));
	_address.sin_family = AF_INET;
	_address.sin_port = htons(static_cast<short>(port));

	bool ret = true;

	if(address == NULL || address[0] == '\0') {
		_address.sin_addr.s_addr = htonl(INADDR_ANY);		
	} else {
		char c;
		const char *p = address;	
		bool isIP = true;
		
		while((c =(*p++)) != '\0') {
			if (( c != '.') && (!((c >= '0') && (c <= '9')))) {
				isIP = false;	
				break;	
			}	
		}

		if (isIP) {
    		_address.sin_addr.s_addr = inet_addr(address);
		} else {
			_dnsMutex.lock();
			
        	struct hostent *myHost = gethostbyname(address);
			if (myHost != NULL) {
    			memcpy(&(_address.sin_addr), *(myHost->h_addr_list),
           		sizeof(struct in_addr));
			} else {
            	ret = false;
        	}

			_dnsMutex.unlock();
		}		
	}

	return ret;
}

bool Socket::checkSocketHandle() {
	if (_socketHandle == -1 && (_socketHandle = socket(AF_INET, SOCK_STREAM,0)) == -1) {
		return false;	
	}

	return true;	
}

bool Socket::connect() {
	if (!checkSocketHandle()) {
		return false;	
	}

	return (0 == ::connect(_socketHandle, (struct sockaddr*)&_address, sizeof(_address)));
}

void Socket::close() {
	if ( _socketHandle != -1) {
		::close(_socketHandle);	
		_socketHandle = -1;	
	}
}

void Socket::shutdown() {
	if(_socketHandle != -1) {
		::shutdown(_socketHandle,SHUT_WR);	
	}
}	

void Socket::setUp(int socketHandle, struct sockaddr *hostAddress) {
	close();
	_socketHandle = socketHandle;
	memcpy(&_address, hostAddress, sizeof(_address));
}

int Socket::getSocketHandle() {
	return _socketHandle;
}	

ioComponent *Socket::getIoComponent() {
	return _iocomponent;
}

void Socket::setIoComponent(ioComponent *ioc) {
	_iocomponent = ioc;
}

int Socket::write(const void *data, int len) {
	if(_socketHandle == -1) {
		return -1;	
	}

	int ret;
	do {
		ret = ::write(_socketHandle,data,len);
		if (ret >0) {
			;
		}	
	} while(ret < 0 && errno == EINTR);

	return ret;
}

int Socket::read(void *data, int len) {
	if (_socketHandle == -1) {
		return -1;	
	} 

	int ret;
	do {
		ret = ::read(_socketHandle,data,len);
		if(ret >0) {
			;
		}	
	} while(ret <0 && errno == EINTR);

	return ret;	
}

bool Socket::setKeepAlive(bool on) {
	return setIntOption(SO_KEEPALIVE, on ? 1:0);	
}

bool Socket::setReuseAddress(bool on) {
	return setIntOption(SO_REUSEADDR, on ? 1:0);	
}

bool Socket::setSoLinger(bool doLinger, int seconds) {
	bool ret = false;

	struct linger lingerTime;
	lingerTime.l_onoff = doLinger ? 1 : 0;
	lingerTime.l_linger = seconds;

	if (checkSocketHandle()) {
    	ret = (setsockopt(_socketHandle, SOL_SOCKET, SO_LINGER,
                (const void *)(&lingerTime), sizeof(lingerTime)) == 0);
	}	

	return ret;	
}
	
bool Socket::setTcpNoDelay(bool noDelay) {
	bool ret = false;
	int noDelayInt = noDelay ? 1 : 0;

	if (checkSocketHandle()) {
		ret = (setsockopt(_socketHandle, IPPROTO_TCP, TCP_NODELAY,
                (const void *)(&noDelayInt), sizeof(noDelayInt)) == 0);
	}

	return ret;
}

bool Socket::setTcpQuickAck(bool quickAck) {
	bool ret = false;
	int quickAckInt = quickAck ? 1 : 0;

	if (checkSocketHandle()) {
		ret = (setsockopt(_socketHandle, IPPROTO_TCP, TCP_QUICKACK,
			    (const void *)(&quickAckInt), sizeof(quickAckInt)) == 0);
	}

	return ret;
}

bool Socket::setIntOption (int option, int value) {
	bool ret=false;

	if (checkSocketHandle()) {
		ret = (setsockopt(_socketHandle, SOL_SOCKET, option,
			       (const void *)(&value), sizeof(value)) == 0);
	}

	return ret;
}

bool Socket::setSoBlocking(bool blockingEnabled) {
	bool ret=false;

	if (checkSocketHandle()) {
		int flags = fcntl(_socketHandle, F_GETFL, NULL);
		if (flags >= 0) {
			if (blockingEnabled) {
    			flags &= ~O_NONBLOCK; // clear nonblocking
			} else {
    			flags |= O_NONBLOCK;  // set nonblocking
			}

			if (fcntl(_socketHandle, F_SETFL, flags) >= 0) {
    			ret = true;
			}
		}
	}

	return ret;
}

std::string Socket::getAddr() {
	char dest[32];
	unsigned long ad = ntohl(_address.sin_addr.s_addr);

	sprintf(dest, "%d.%d.%d.%d:%d",
			static_cast<int>((ad >> 24) & 255),
    		static_cast<int>((ad >> 16) & 255),
    		static_cast<int>((ad >> 8) & 255),
    		static_cast<int>(ad & 255),
    		ntohs(_address.sin_port));

	return dest;
}

uint64_t Socket::getId() {
	uint64_t ip = ntohs(_address.sin_port);
	ip <<= 32;
	ip |= _address.sin_addr.s_addr;

	return ip;
}

uint64_t Socket::getPeerId() {
	if (_socketHandle == -1) {
		return 0;
	} 

	struct sockaddr_in peer;
	socklen_t length = sizeof(peer);

	if (getpeername(_socketHandle,(struct sockaddr*)&peer, &length) == 0) {
		return sys::netUtil::ipToAddr(peer.sin_addr.s_addr, ntohs(peer.sin_port));
	}

	return 0;
}

int Socket::getLocalPort() {
	if (_socketHandle == -1) {
		return -1;
	}

	int result = -1;
	struct sockaddr_in addr;
	socklen_t len = sizeof(addr);

	if (getsockname(_socketHandle, (struct sockaddr*)(&addr), &len) == 0) {
		result = ntohs(addr.sin_port);
	}

	return result;
}

int Socket::getSoError () {
	if (_socketHandle == -1) {
    	return EINVAL;
	}

	int lastError = Socket::getLastError();
	int  soError = 0;
	socklen_t soErrorLen = sizeof(soError);

	if (getsockopt(_socketHandle, SOL_SOCKET, SO_ERROR, (void *)(&soError), &soErrorLen) != 0) {
    	return lastError;
	}

	if (soErrorLen != sizeof(soError)) {
    	return EINVAL;
	}

	return soError;
}

}//namespace net

