
#ifndef NET_IPACKET_FACTORY_H_
#define NET_IPACKET_FACTORY_H_

namespace net {

class packetFactory {
	
public:
    virtual ~packetFactory() {};
    virtual packet *createPacket(int pcode) = 0;
};

}//namespace net

#endif//NET_IPACKET_FACTORY_H_

