#ifndef TCPACCEPTER_H_
#define TCPACCEPTER_H_

namespace net {

class tcpAccepter : public ioComponent {
	
public:
	tcpAccepter(transport *owner, Socket *socket,
		        packetStreamer *streamer, serverAdapter *_serverAdapter);

	bool init(bool isServer = false);

	bool handleReadEvent();

	bool handleWriteEvent() {
		return true;	
	}

	void checkTimeOut(int64_t now);

private:
	packetStreamer *_streamer;
	serverAdapter *_serverAdapter;	
};

}//namespace net

#endif//TCPACCEPTER_H_

