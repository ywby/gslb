
#include "net.h"

namespace net {

//int defaultPacketStreamer::_nPacketFlag = NET_PACKET_FLAG;

defaultPacketStreamer::DefaultPacketStreamer() {}

defaultPacketStreamer::defaultPacketStreamer(packetFactory *factory) : packetStreamer(factory) {}

defaultPacketStreamer::~defaultPacketStreamer() {}

void defaultPacketStreamer::setPacketFactory(packetFactory *factory) {
    _factory = factory;
}

bool defaultPacketStreamer::getPacketInfo(dataBuffer *input, packetHeader *header, bool *broken) {
    if (_existPacketHeader) {
        if (input->getDataLen() < (int)(4 * sizeof(int)))
            return false;

        int flag = input->readInt32();
        header->_chid = input->readInt32();
        header->_pcode = input->readInt32();
        header->_dataLen = input->readInt32();

        if (flag != defaultPacketStreamer::_nPacketFlag || header->_dataLen < 0 ||
                header->_dataLen > 0x4000000) { // 64M
            //SYS_LOG(ERROR, "stream error: %d", header->dataLen);
            *broken = true;
        }
    } else if (input->getDataLen() == 0) {
        return false;
    }

    return true;
}

packet *defaultPacketStreamer::decode(dataBuffer *input, packetHeader *header) {
    assert(_factory != NULL);
    packet *packet = _factory->createPacket(header->_pcode);
    if (packet != NULL) {
        if (!packet->decode(input, header)) {
            packet->free();
            packet = NULL;
        }
    } else {
        input->drainData(header->_dataLen);
    }

    return packet;
}

bool defaultPacketStreamer::encode(packet *packet, dataBuffer *output) {
   	packetHeader *header = packet->getPacketHeader();
    
	int oldLen = output->getDataLen();
	int dataLenOffset = -1;
	int headerSize = 0;

    if (_existPacketHeader) {
        output->writeInt32(defaultPacketStreamer::_nPacketFlag);
        output->writeInt32(header->_chid);
        output->writeInt32(header->_pcode);
        dataLenOffset = output->getDataLen();
        output->writeInt32(0);
        headerSize = 4 * sizeof(int);
    }
    
	if (packet->encode(output) == false) {
        //SYS_LOG(ERROR, "encode error");
        output->stripData(output->getDataLen() - oldLen);
        return false;
    }
    
	header->_dataLen = output->getDataLen() - oldLen - headerSize;
	if (dataLenOffset >= 0) {
    	unsigned char *ptr = (unsigned char *)(output->getData() + dataLenOffset);
    	output->fillInt32(ptr, header->_dataLen);
	}

    return true;
}

void defaultPacketStreamer::setPacketFlag(int flag) {
    defaultPacketStreamer::_nPacketFlag = flag;
}

}//namespace net

