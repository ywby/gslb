#ifndef NET_PACKETSTREAMER_H_
#define NET_PACKETSTREAMER_H_

namespace net {

class packetStreamer {

public:
    packetStreamer() {
		_factory = NULL;
		_existPacketHeader = true;
	}

	packetStreamer(packetFactory *factory) {
		_factory = factory;
		_existPacketHeader = true;
	}

	virtual ~packetStreamer() {}

	virtual bool getPacketInfo(dataBuffer *input, packetHeader *header, bool *broken) = 0;

	virtual packet *decode(dataBuffer *input, packetHeader *header) = 0;
	virtual bool encode(packet *packet, dataBuffer *output) = 0;

	bool existPacketHeader() {
		return _existPacketHeader;
	}

public:
	int pattern;	//运行模式
	
protected:
	packetFactory *_factory;  
	bool _existPacketHeader;    
};

}//namespace net


#endif//NET_PACKETSTREAMER_H_

