#ifndef NET_IPACKETHANDLER_H_
#define NET_IPACKETHANDLER_H_

namespace net {

class packetHandler {
	
public:
    enum HPRetCode {
        KEEP_CHANNEL  = 0,
        CLOSE_CHANNEL = 1,
        FREE_CHANNEL  = 2
    };

    virtual ~packetHandler() {}
    virtual HPRetCode handlePacket(packet *packet, void *args) = 0;
};

}//namespace net

#endif//NET_IPACKETHANDLER_H_

