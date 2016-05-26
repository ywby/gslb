#ifndef NET_TCPCOMPONENT_H_
#define NET_TCPCOMPONENT_H_

namespace net {

class tcpComponent : public ioComponent {
	
public:	
	tcpComponent(transport *owner, Socket *socket,
			     packetStreamer *streamer, 
			     serverAdapter *_serverAdapter, 
			     bool isServer,bool appTcp);
	
	~tcpComponent();

	bool init(bool isServer = false);

	void close();

	bool handleWriteEvent();

	bool handleReadEvent();

	tcpConnection *getConnection() {
		return _connection;	
	}		

	void checkTimeOut(int64_t now);

	bool socketConnect();

public:
	tcpConnection *_connection;

private:
	//tcpConnection *_connection;
	int64_t _startConnectTime;				
};

}//namespace net

#endif//NET_TCPCOMPONENT_H_

