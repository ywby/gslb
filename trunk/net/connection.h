
#ifndef NET_CONNECTION_H_
#define NET_CONNECTION_H_

#define READ_WRITE_SIZE 8192
#ifndef UNUSED
#define UNUSED(v) ((void)(v))
#endif

namespace net {

class connection {
    
public:
	connection(Socket *socket, packetStreamer *streamer, serverAdapter *_serverAdapter, bool appTcp);

	virtual ~connection();

	void setServer(bool isServer) {
   	 	_isServer = isServer;
	}

	void setIoComponent(ioComponent *ioc) {
    	_iocomponent = ioc;
	}

	ioComponent *getIoComponent() {
    	return _iocomponent;
	}

    void setDefaultPacketHandler(packetHandler *ph) {
    	_defaultPacketHandler = ph;
	}

	bool postPacket(packet *packet, packetHandler *_packetHandler = NULL, void *args = NULL, bool noblocking = true);

	bool handlePacket(dataBuffer *input, packetHeader *header);


	//bool checkTimeout(int64_t now);

	virtual bool writeData() = 0;
	virtual bool readData() = 0;

	virtual void setWriteFinishClose(bool v) {
  		UNUSED(v);
	}

	void setQueueTimeout(int queueTimeout) {
    	_queueTimeout = queueTimeout;
	}

	virtual void clearOutputBuffer() {
    	;
	}

	void setQueueLimit(int limit) {
    	_queueLimit = limit;
	}

	bool isConnectState();

	uint64_t getServerId() {
    	if (_socket) {
        	return _socket->getId();
    	}

    	return 0;
	}

	uint64_t getPeerId() {
    	if (_socket) {
        	return _socket->getPeerId();
    	}

    	return 0;
	}

	int getLocalPort() {
    	if (_socket) {
        	return _socket->getLocalPort();
    	}

    	return -1;
	}

public:
    bool _gotFirst;            		// 得到第一条指令 	
    Socket *_socket;                // Socket句柄
    bool _appTcp;                   //app端tcp连接	
    int appID;               		// app ID
    int userID;               		// user ID

protected:
    void disconnect();

protected:
    packetHandler *_defaultPacketHandler;  // connection的默认的packet handler

    bool _isServer;                         // 是服务器端

    ioComponent *_iocomponent;
    packetStreamer *_streamer;             // Packet解析
    serverAdapter *_serverAdapter;         // 服务器适配器

    packetQueue _outputQueue;               // 发送队列
    packetQueue _inputQueue;                // 发送队列
    packetQueue _myQueue;                   // 在write中处理时暂时用

    sys::threadCond _outputCond;         // 发送队列的条件变量
    
    int _queueTimeout;                      // 队列超时时间
    int _queueTotalSize;                    // 队列总长度
    int _queueLimit;                        // 队列最长长度, 如果超过这个值post进来就会被wait
};

}//namespace net 

#endif//NET_CONNECTION_H_

