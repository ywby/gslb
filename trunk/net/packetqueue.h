#ifndef NET_PACKET_QUEUE_H_
#define NET_PACKET_QUEUE_H_

namespace net {

class packetQueue {
    friend class packetQueueThread;
public:
    packetQueue();
	~packetQueue();

	packet *pop();

	void clear();

	void push(packet *_packet);

	int size();

	bool empty();

	void moveTo(packetQueue *destQueue);

	packet *getTimeoutList(int64_t now);

	packet *getPacketList();

	packet *head() {
		return _head;
	}
    
	packet* tail() {
        return _tail;
    }
    
protected:
    packet *_head;  
    packet *_tail;  
    int _size;    
};

}//namespace net

#endif//NET_PACKET_QUEUE_H_

