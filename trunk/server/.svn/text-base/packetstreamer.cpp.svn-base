#include "define.h"

namespace maaserver {
maaPacketStreamer::maaPacketStreamer() {
}

maaPacketStreamer::maaPacketStreamer(maanet::packetFactory* factory) 
: packetStreamer(factory) {
}

maaPacketStreamer::~maaPacketStreamer() {
}

void maaPacketStreamer::set_packet_factory(maanet::packetFactory* factory) {
	_factory = factory;
}

//指令头分析
bool maaPacketStreamer::getPacketInfo(maanet::dataBuffer* input, maanet::packetHeader* header, bool* broken) {
	if (input->getDataLen() < (int32_t)MAA_PACKET_HEADER_SIZE) {
		//MAASYS_LOG(DEBUG, "datalen =%d, headerLen=%d", input->getDataLen(), MAA_PACKET_HEADER_SIZE);
		return false;	
	}
	
	//MAASYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d", input->readInt32(), input->readInt32(),input->readInt32(),input->readInt32());
	//maanet::packetHeader maaHeader;
	//memcpy(&maaHeader,input->getData(),sizeof(MAA_PACKET_HEADER_SIZE));
	//input->drainData(MAA_PACKET_HEADER_SIZE);	

	header->type = input->readInt32();	
	header->dataLen = input->readInt32();	
	header->appID = input->readInt32();	
	header->userID = input->readInt32();	
		
	//MAASYS_LOG(DEBUG, "first header: type =%d, datalen =%d,appID =%d, userID =%d", header->type, header->dataLen,header->appID,header->userID);
	if (header->dataLen  <= 0 || header->dataLen > 0x4000000) { // 64M
		*broken = true;
	}	

	return true;
}

maanet::packet* maaPacketStreamer::decode(maanet::dataBuffer* input, maanet::packetHeader* header ) {
	if (input == NULL || header == NULL) {
		return NULL;	
	}

	//解析不出来头，则是纯代理数据	
	if (header->dataLen == 0) {
		header->dataLen = input->getDataLen();	
		if (pattern == 1 ) {		
			header->type = 2;
		} else if (pattern == 3) {	
			header->type = 3;
		}	
	}	

	maanet::packet* _packet = createPacket(input, header);
	if (_packet != NULL) {
		if (!_packet->decode(input, header)) {
			_packet->free();
			_packet = NULL;
		}
	} else {
		input->drainData(header->dataLen);
	}

	return _packet;
}
	
maanet::packet* maaPacketStreamer::createPacket(maanet::dataBuffer* input, maanet::packetHeader* header) {
	if (input == NULL  || header == NULL) {
		return NULL;	
	}

	//if (header->_dataLen == 0) {
	//	header->type = 2;
	//	header->_dataLen = input->getDataLen();	
	//}	

	message *message_ = new message();

	message_->setMessageType(header->type);
	message_->setDataLen(header->dataLen);
	message_->setAppID(header->appID);
	message_->setUserID(header->userID);

	MAASYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d", header->type, header->dataLen,header->appID,header->userID);
	
	return message_;	
}


bool maaPacketStreamer::encode(maanet::packet* packet, maanet::dataBuffer* output) {
	message* msg = (message*) packet;
	maanet::packetHeader* _header = msg->getPacketHeader();
	int old_len = output->getDataLen();

	//int header_size = 0;
	//maanet::packetHeader _header;
	//packetHeader.length_ = msg->get_raw_size();
	//packetHeader.type = header->type;		
	//packetHeader.appID = header->appID;		
	//packetHeader.userID = header->userID;		
	//MAASYS_LOG(DEBUG, "header: type =%d, datalen =%d,appID =%d, userID =%d,data=%s", (packet->_packetHeader).type, (packet->_packetHeader).dataLen,(packet->_packetHeader).appID,(packet->_packetHeader).userID,msg->raw_data_);

	if ((pattern == 2) || 
		(pattern == 1 && _header->type != 3) || 
		(pattern ==3 && _header->type !=2)) {
		output->writeBytes(_header, MAA_PACKET_HEADER_SIZE);

		//output->drainData(MAA_PACKET_HEADER_SIZE);	
		MAASYS_LOG(DEBUG, "response header: type =%d, datalen =%d,appID =%d, userID =%d rawData=%s",
		 _header->type, _header->dataLen,_header->appID,_header->userID,msg->raw_data_);
	}
	
	if (msg->encode(output) == false) {
    	output->stripData(output->getDataLen() - old_len);
		//delete msg;
    	return false;
  	}
  	
	//delete msg;
	return true;		
}

}//namespace maaserver

