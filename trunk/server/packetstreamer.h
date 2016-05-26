#ifndef MAASERVER_MAAPACKETSTREAMER_H_
#define MAASERVER_MAAPACKETSTREAMER_H_

namespace maaserver {

class maaPacketStreamer: public maanet::packetStreamer {

public:
	maaPacketStreamer();
	maaPacketStreamer(maanet::packetFactory* factory);
	~maaPacketStreamer();

	void set_packet_factory(maanet::packetFactory* factory);
	bool getPacketInfo(maanet::dataBuffer* input, maanet::packetHeader* header, bool* broken);
	
	net::packet* decode(maanet::dataBuffer* input, maanet::packetHeader* header);
	bool encode(maanet::packet* _packet, maanet::dataBuffer* output);
	
	maanet::packet* createPacket(maanet::dataBuffer* input, maanet::packetHeader* header);
};

}//namespace maaserver

#endif//MAASERVER_MAAPACKETSTREAMER_H_

