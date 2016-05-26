#ifndef NET_PACKET_QUEUE_THREAD_H
#define NET_PACKET_QUEUE_THREAD_H

namespace net {

class packetQueueHandler {

public:
    virtual ~packetQueueHandler() {}
    virtual bool handlePacketQueue(packet *_packet, void *args) = 0;
};

class packetQueueThread : public sys::defaultRunnable {
	
public:
    packetQueueThread();
    packetQueueThread(int threadCount, packetQueueHandler *handler, void *args);
	~packetQueueThread();

	void setThreadParameter(int threadCount, packetQueueHandler *handler, void *args);

	void stop(bool waitFinish = false);

	bool push(packet *packet, int maxQueueLen = 0, bool block = true);

	void pushQueue(packetQueue &packetQueue, int maxQueueLen = 0);

	void run(sys::cThread *thread, void *arg);

	void setStatSpeed();

	void setWaitTime(int t);

	packet *head() {
		return _queue.head();
	}

	packet *tail() {
		return _queue.tail();
	}
	
	size_t size() {
		return _queue.size();
	}

private:
   	void checkSendSpeed();

private:
    packetQueue _queue;
	packetQueueHandler *_handler;
	sys::threadCond _cond;
	sys::threadCond _pushcond;

	void *_args;
	bool _waitFinish;      

	int _waitTime;
	int64_t _speed_t1;
	int64_t _speed_t2;
	int64_t _overage;

	bool _waiting;
};

}//namespace net

#endif//NET_PACKET_QUEUE_THREAD_H

