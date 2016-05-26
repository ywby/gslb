#ifndef NET_ISERVERADAPTER_H
#define NET_ISERVERADAPTER_H

#ifndef UNUSED
    #define UNUSED(v) ((void)(v))
#endif

namespace net {

class serverAdapter {
    friend class connection;
    friend class tcpConnection;
public:
    virtual packetHandler::HPRetCode  handlePacket(connection *_connection, packet *_packet) = 0;
    
	virtual bool handleBatchPacket(connection *_connection, packetQueue &_packetQueue) {
        UNUSED(_packetQueue);
        UNUSED(_connection);
        return false;
    }
    
	serverAdapter() {
        _batchPushPacket = false;
    }
    
	virtual ~serverAdapter() {
    }
    
	void setBatchPushPacket(bool value) {
        _batchPushPacket = value;
    }

private:
    bool _batchPushPacket;          
	//int pattern;  //运行模式
};

}//namespace net

#endif//NET_ISERVERADAPTER_H
