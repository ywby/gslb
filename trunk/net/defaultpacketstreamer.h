
#ifndef NET_DEFAULT_PACKET_STREAMER_H_
#define NET_DEFAULT_PACKET_STREAMER_H_

namespace net {

class defaultPacketStreamer : public packetStreamer {

public:
	defaultPacketStreamer();

	defaultPacketStreamer(packetFactory *factory);

	~defaultPacketStreamer();

	void setPacketFactory(packetFactory *factory);

	bool getPacketInfo(dataBuffer *input, packetHeader *header, bool *broken);

	packet *decode(dataBuffer *input, packetHeader *header);

	bool encode(packet *packet, dataBuffer *output);

	static void setPacketFlag(int flag);

public:
    static int _nPacketFlag;
};

}//namespace net

#endif//NET_DEFAULT_PACKET_STREAMER_H_

