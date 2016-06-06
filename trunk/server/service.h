#ifndef SERVER_SERVICE_H_
#define SERVER_SERVICE_H_

#include "define.h"

namespace server {

class service:  public net::serverAdapter, 
                public net::packetQueueHandler, 
                public sys::runnable 
{
public:
	service();
    
	virtual ~service();
	
    int start();
	
    int stop();
	
    int wait();

public:
	net::packetHandler::HPRetCode handlePacket(net::connection *_connection,  net::packet *_packet);
    
    bool handlePacketQueue(net::packet *_packet, void *args);

	globalInformation* get_global_info() 
    {
		return &global_info;
	}
	
	net::packetQueueThread* get_packet_queue_thread() 
    {
		return &taskQueueThread_;
	}

private:
	PacketStreamer streamer_;

	globalInformation global_info;
	
    net::transport transport_;

	int32_t read_task_queue_size_;
	    
    int32_t write_task_queue_size_;
	    
    net::packetQueueThread taskQueueThread_;
	
    void run(sys::cThread *thread, void *arg);

private:
    void initHandleThreads();	
	
    int initGlobalInfo();
	
};

}//namespace server

#endif//SERVER_SERVICE_H_



