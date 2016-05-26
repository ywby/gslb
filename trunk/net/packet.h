#ifndef NET_PACKET_H_
#define NET_PACKET_H_

namespace net {

class packetHeader {  
 
public:
   	/* 
	uint32_t _chid;         // 通道ID
    	int _pcode;             // 数据包类型
    	int _dataLen;           // 数据包body长度(除头信息外)
	*/
	uint32_t type; 		//指令类型
	uint32_t dataLen;	//指令body长度
	uint32_t appID;		//APPID
	uint32_t userID;	//用户ID
};

class packet {
    friend class packetQueue;

public:
    packet();
    virtual ~packet();



    packetHeader *getPacketHeader() {
        return &_packetHeader;
    }

    /*
     * 设置数据包header info
     */
    void setPacketHeader(packetHeader *header) {
        if (header) {
            memcpy(&_packetHeader, header, sizeof(packetHeader));
        }
    }

    /*
     * 释放自己
     */
    virtual void free() {
        delete this;
    }

    /*
     * 是否数据包
     */
    virtual bool isRegularPacket() {
        return true;
    }

    virtual bool encode(dataBuffer *output) = 0;

    virtual bool decode(dataBuffer *input, packetHeader *header) = 0;

    /*
     * 超时时间
     */
    int64_t getExpireTime() const {
        return _expireTime;
    }

    /*
     * 设置过期时间
     *
     * @param milliseconds 毫秒数, 0为永不过期
     */
    void setExpireTime(int milliseconds);


    /*
     * 得到next
     */
    packet *getNext() const {
        return _next;
    }
	
    int addRef() {
		return atomic_add_return(1, &_refcount);	
	}

	int subRef() {
		return atomic_sub_return(1, &_refcount);	
	}

	int getRef() {
		return atomic_read(&_refcount);	
	}

public:
    packetHeader _packetHeader; // 数据包的头信息
    atomic_t _refcount;
	
protected:
    int64_t _expireTime;        // 到期时间
    //Channel *_channel;
    packet *_next;              // 用在packetqueue链表
};

}//namespace net

#endif//NET_PACKET_H_

