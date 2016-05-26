#include "net.h"

namespace net {

tcpComponent::tcpComponent(transport *owner, Socket *socket,
				packetStreamer *streamer,serverAdapter *_serverAdapter,bool isServer,bool appTcp) 
: ioComponent(owner,socket) {
	_connection = new tcpConnection(socket, streamer, _serverAdapter,appTcp);
	_connection->setIoComponent(this);
	_startConnectTime = 0;
	_isServer = false;
}

tcpComponent::~tcpComponent() {
	if (_connection) {
		_connection->setIoComponent(NULL);

		delete _connection;
		_connection = NULL;	
	}	
}

bool tcpComponent::init(bool isServer) {
	_socket->setSoBlocking(false);
	_socket->setSoLinger(false,0);
	_socket->setReuseAddress(true);	
	_socket->setIntOption(SO_KEEPALIVE, 1);
	_socket->setIntOption(SO_SNDBUF, 640000);
	_socket->setIntOption(SO_RCVBUF, 640000);

	if (!isServer) {
		if (!socketConnect() && _autoReconn == false) {
    		return false;
		}
	} else {
    	_state = NET_CONNECTED;
	}

	_connection->setServer(isServer);
	_isServer = isServer;

	return true;
}

bool tcpComponent::socketConnect() {
	if (_state == NET_CONNECTED || _state == NET_CONNECTING) {
    	return true;
	}
	
	_socket->setSoBlocking(false);

	if (_socket->connect()) {
		if (_socketEvent) {
			_socketEvent->addEvent(_socket, true, true);
		}

		_state = NET_CONNECTED; 
		_startConnectTime = sys::timeUtil::getTime();
	} else {
		int error = Socket::getLastError();
		if (error == EINPROGRESS || error == EWOULDBLOCK) {

			_state = NET_CONNECTING;
            if (_socketEvent) {
                _socketEvent->addEvent(_socket, true, true);
            }
		} else {
			_socket->close();
			_state = NET_CLOSED;
			std::cout<< "connet to " <<_socket->getAddr().c_str() << " error" << " errno =" << error<< std::endl ;
			return false;
		}
	}

	printf("tcp client connect end ....\n");
	return true;
}

void tcpComponent::close() {
	if (_socket) {
		_socketEvent->removeEvent(_socket);	
	}	

	if (_connection && isConnectState()) {
		_connection->setDisconnState();	
	}

	_socket->close();

	if (_connection) {
		_connection->clearInputBuffer();		
	}	

	_state = NET_CLOSED;
}

bool tcpComponent::handleWriteEvent() {
	_lastUseTime = sys::timeUtil::getTime();

	bool ret = true;
	if (_state == NET_CONNECTED) {
		ret = _connection->writeData();	
	}
	else if (_state == NET_CONNECTING) {
		int error = _socket->getSoError();
		if (error == 0) {
			enableWrite(true);
			_connection->clearOutputBuffer();
			_state = NET_CONNECTED;	
		}
		else {
			//sys.log(链接失败)
			if (_socketEvent) {
				_socketEvent->removeEvent(_socket);	
			}

			_socket->close();
			_state = NET_CLOSED;	
		}	
	}	

	return ret;
}

bool tcpComponent::handleReadEvent() {
	//更新收到时间时间	
	_lastUseTime = sys::timeUtil::getTime();

	bool ret = false;
	if (_state == NET_CONNECTED) {
		ret = _connection->readData();	
	}	

	return ret;	
}


void tcpComponent::checkTimeOut(int64_t now) {
	if (_state == NET_CONNECTING) {
		if (_startConnectTime > 0 && 
		    _startConnectTime < (now - static_cast<int64_t>(2000000))) { // 连接超时 2 秒

			_state = NET_CLOSED;
			_socket->shutdown();
		}
	} 
	else if (_state == NET_CONNECTED && 
	         _isServer == true && _autoReconn == false) { // 连接的时候, 只用在服务器端

		int64_t idle = now - _lastUseTime;
		if (idle > static_cast<int64_t>(900000000)) { // 空闲15min断开
			_state = NET_CLOSED;
			SYS_LOG(INFO, "%s 空闲了: %ld (s) 被断开.", 
				       _socket->getAddr().c_str(), (idle/static_cast<int64_t>(1000000)));
			_socket->shutdown();
		}
	}

	// 超时检查
	//_connection->checkTimeout(now);
}

}//namespace net

