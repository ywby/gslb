
#include "net.h"

namespace net {

connection::connection(Socket *socket, packetStreamer *streamer, serverAdapter *serverAdapter_, bool appTcp) {
    _socket = socket;
    _streamer = streamer;
    _serverAdapter = serverAdapter_;
    _defaultPacketHandler = NULL;
    _iocomponent = NULL;
    _queueTimeout = 5000;
    _queueLimit = 50;
    _queueTotalSize = 0;

    _gotFirst = true;         
    _appTcp = appTcp; 
}

connection::~connection() {
    disconnect();
    _socket = NULL;
    _iocomponent = NULL;
}

void connection::disconnect() {
    _outputCond.lock();
    _myQueue.moveTo(&_outputQueue);
    _outputCond.unlock();
    //checkTimeout(TBNET_MAX_TIME);
}

bool connection::postPacket(packet *packet, packetHandler *_packetHandler, void *args, bool noblocking) {
	if (!isConnectState()) {
    	if (_iocomponent == NULL ||  _iocomponent->isAutoReconn() == false) {
        		return false;
        } else if (_outputQueue.size()>10) {
            	return false;
        } else {
            tcpComponent *ioc = dynamic_cast<tcpComponent*>(_iocomponent);
            bool ret = false;
            if (ioc != NULL) {
            	_outputCond.lock();
            	ret = ioc->init(false);
            	_outputCond.unlock();
            }

            if (!ret) 
                return false;
        }
    }
    
	// 如果是client, 并且有queue长度的限制
    _outputCond.lock();
    _queueTotalSize = _outputQueue.size()  + _myQueue.size();
	if (!_isServer && _queueLimit > 0 && noblocking && _queueTotalSize >= _queueLimit) {
    	_outputCond.unlock();
    	return false;
    }
    _outputCond.unlock();
	
	packet->setExpireTime(_queueTimeout);           // 设置超时
	
	_outputCond.lock();
	//SYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d", (packet->_packetHeader).type, (packet->_packetHeader).dataLen,(packet->_packetHeader).appID,(packet->_packetHeader).userID);	
	// 写入到outputqueue中
	_outputQueue.push(packet);
	if (_iocomponent != NULL && _outputQueue.size() == 1U) {
		_iocomponent->enableWrite(true);
	}
	
	_outputCond.unlock();
	if (!_isServer && _queueLimit > 0) {
		_outputCond.lock();

		_queueTotalSize = _outputQueue.size()  + _myQueue.size();

		if ( _queueTotalSize > _queueLimit && noblocking == false) {
			bool *stop = NULL;
			if (_iocomponent && _iocomponent->getOwner()) {
				stop = _iocomponent->getOwner()->getStop();
			}
			while (_queueTotalSize > _queueLimit && stop && *stop == false) {
				if (_outputCond.wait(1000) == false) {
					if (!isConnectState()) {
						break;
					}
					_queueTotalSize = _outputQueue.size()  + _myQueue.size();
				}
			}
		}
		_outputCond.unlock();
	}

	if (_isServer && _iocomponent) {
		_iocomponent->subRef();
	}

	return true;
}


bool connection::handlePacket(dataBuffer *input, packetHeader *header) {
	packet *_packet;
	_packet = _streamer->decode(input, header);
	if (_packet == NULL) {
		return false;	
	} 
        
	if (_iocomponent) {
	   _iocomponent->addRef();
   	} 

	//SYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d", header->type, header->dataLen,header->appID,header->userID);
	//SYS_LOG(DEBUG, "packet type =%d, datalen =%d,appID =%d, userID =%d", _packet->_packetHeader.type, _packet->_packetHeader.dataLen,_packet->_packetHeader.appID,_packet->_packetHeader.userID);

	_serverAdapter->handlePacket(this, _packet);
    	
	return true;
}

/**
 * 连接状态
 */
bool connection::isConnectState() {
	if (_iocomponent != NULL) {
        return _iocomponent->isConnectState();
    }

    return false;
}
}//namespace net


