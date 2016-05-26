#include "net.h"

namespace net {

tcpConnection::tcpConnection(Socket *socket, packetStreamer *streamer,
                             serverAdapter *_serverAdapter, bool appTcp)
: connection(socket, streamer, _serverAdapter, appTcp) {
	_gotHeader = false;
	_writeFinishClose = false;
	memset(&_packetHeader, 0, sizeof(_packetHeader));
}

tcpConnection::~tcpConnection() {
}

bool tcpConnection::writeData() {
    //_outputQueue copy to  _myQueue
	_outputCond.lock();
	_outputQueue.moveTo(&_myQueue);

	if (_myQueue.size() == 0 && _output.getDataLen() == 0) { // 返回
		_iocomponent->enableWrite(false);
		_outputCond.unlock();
		return true;
	}
	_outputCond.unlock();

	packet *packet;
	int ret;
	int writeCnt = 0;
	int myQueueSize = _myQueue.size();
	//int pattern = _serverAdapter->pattern; 

	do {
		// 写满到
		while (_output.getDataLen() < READ_WRITE_SIZE) {

			if (myQueueSize == 0)
				break;

			packet = _myQueue.pop();
			myQueueSize --;
			//SYS_LOG(DEBUG, "myQueueSize =%d , useID=%d, fd=%d",myQueueSize, packet->_packetHeader.userID,_socket->_socketHandle);	

			_streamer->encode(packet, &_output);
			packet->free();
		}

		if (_output.getDataLen() == 0) {
			break;
		}

		// write data
		ret = _socket->write(_output.getData(), _output.getDataLen());
		if (ret > 0) {
			_output.drainData(ret);
		}

		writeCnt ++;
	} while (ret > 0 && _output.getDataLen() == 0 && myQueueSize>0 && writeCnt < 10);

	// 紧缩
	_output.shrink();

	_outputCond.lock();
	int queueSize = _outputQueue.size() + _myQueue.size() + (_output.getDataLen() > 0 ? 1 : 0);
	if ((queueSize == 0 || _writeFinishClose) && _iocomponent != NULL) {
		_iocomponent->enableWrite(false);
	}
	_outputCond.unlock();

	if (_writeFinishClose) {
		//SYS.log(ERROR, "主动断开.");
		return false;
	}

	// 如果是client, 并且有queue长度的限制
	if (!_isServer && _queueLimit > 0 &&  _queueTotalSize > _queueLimit) {
		_outputCond.lock();

		// ywby notice 
		//_queueTotalSize = queueSize + _channelPool.getUseListCount();
		_queueTotalSize = queueSize ;

		if (_queueTotalSize <= _queueLimit) {
			_outputCond.broadcast();
		}

		_outputCond.unlock();
	}

	return true;
}

bool tcpConnection::readData() {
	_input.ensureFree(READ_WRITE_SIZE);

	int ret = _socket->read(_input.getFree(), _input.getFreeLen());

	int freeLen = 0;
	int readCnt = 0;
	bool broken = false;

	//apptcp专有通道或第一条指令，正常解析
	if (_appTcp || _gotFirst ) {

		while (ret > 0) {
			_input.pourData(ret);
			freeLen = _input.getFreeLen();

			while (1) {
				if (!_gotHeader) {
					_gotHeader = _streamer->getPacketInfo(&_input, &_packetHeader, &broken);
				
					if (broken) {
						break;
					}
				}
			
	
				//数据完整，调用handlePacket
				if ( _gotHeader && _input.getDataLen() >= (int32_t)_packetHeader.dataLen) {
					handlePacket(&_input, &_packetHeader);
					
					_gotHeader = false;
					_packetHeader.dataLen = 0;
					//_gotFirst = false;	
				} else {
					break;
				}
			}
		
			if (broken || freeLen > 0 || readCnt >= 10) {
            	break;
        	}

			if (_packetHeader.dataLen - _input.getDataLen() > READ_WRITE_SIZE) {
				_input.ensureFree(_packetHeader.dataLen - _input.getDataLen());
			} else {
				_input.ensureFree(READ_WRITE_SIZE);
			}

			ret = _socket->read(_input.getFree(), _input.getFreeLen());
			readCnt++;	
		}
	}
	else {
		_packetHeader.appID =  appID;	
		_packetHeader.userID = userID;	
		handlePacket(&_input, &_packetHeader);
	}

    _socket->setTcpQuickAck(true);

    _input.shrink();

    if (!broken) {
        if (ret == 0) {
            broken = true;
        } else if (ret < 0) {
            int error  = Socket::getLastError();
            broken = (error != EAGAIN);
        }
    } else {
		//SYS_LOG(ERROR,"get header is error");
        _gotHeader = false;
    }

    return !broken;
}

/**
 * 发送setDisconnState
 */
void tcpConnection::setDisconnState() {
    disconnect();

    if (_defaultPacketHandler && _isServer == false) {
       	//ywby notice 
		//_defaultPacketHandler->handlePacket(&ControlPacket::DisconnPacket, _socket);
    }
}

}//namespace net

