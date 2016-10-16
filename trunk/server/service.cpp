#include "define.h"

namespace server {

service::service() 
{
	memset(&global_info, 0, sizeof(globalInformation));
}

void service::initHandleThreads() 
{
	taskQueueThread_.setThreadParameter(1, this, NULL);	
    taskQueueThread_.start();	
}

service::~service() 
{
}

void service::run(sys::cThread *thread, void *arg) 
{
	std::cout<<"hello"<<endl;	
}	

net::packetHandler::HPRetCode service::handlePacket(net::connection *_connection, net::packet *_packet) 
{
	if (_connection == NULL || _packet == NULL) 
    {
		return net::packetHandler::FREE_CHANNEL;	
	}	
    
    bool ret = true;
	std::map<int, net::connection *>::iterator it_m; 

    int type =  _packet->_packetHeader.type;	
	
	return net::packetHandler::KEEP_CHANNEL;	
}	
	
bool service::handlePacketQueue(net::packet *_packet,void *) 
{
  	bool  ret = true;
	std::map<int, char *>::iterator it_m;
	std::map<char *, net::connection *>::iterator it_n;
    
    int type =  _packet->_packetHeader.type;	

	message* _message = dynamic_cast<message*>(_packet); 
	if (NULL == _message) {
		SYS_LOG(ERROR, "process packet can convert to message\n");
		return true;
	}

	switch (type) {
	case 1:{ 
		switch (global_info.pattern) {
		case 1: {
			_message->replyMessage(_message);
			break;
		} 	
		case 2: {
			break;	
		}
		case 3: {
            SYS_LOG(DEBUG, "TCP data is coming... : appid =%d", _message->getAppID());
			//与源建立连接
			net::connection *oldConn =  _message->getConnection();	

			it_m = appidSourceMap.find(_message->getAppID());	
			if (it_m != appidSourceMap.end()) {
				net::connection *_conn = transport_.connect(it_m->second, &streamer_, false, false); 
				if (_conn != NULL) {
					_conn->userID = _message->getUserID();	
					_conn->appID = _message->getAppID();	
				    
                    SYS_LOG(DEBUG, "create a success connection : addr =%s ", (it_m->second));	

					transport_.socketConnMap.insert(map<int ,net::connection *>::value_type(_message->getUserID(),oldConn));	
				} else {
                    SYS_LOG(ERROR, "create a fail connection    : addr =%s ", (it_m->second));	
				}	
			}	
			break;	
		}
		}
		break;	
	}	
	case 2: {
		switch (global_info.pattern) {	
		case 1:	{
			_message->replyMessage(_message);
			break;	
		}		
		case 2: {	
			break;
		}	
		case 3: {
			_message->replyMessage(_message);
			break;
		}	
		default : {
			break;	
		}	
		}	
		break;
	}	
	case 3: {
		switch (global_info.pattern) {
		case 1:
			_message->replyMessage(_message);
			break;
		case 2:
			break;
		case 3:
			break;
		default :
			break;
		}	
		break;
	}	
	default: {
		SYS_LOG(ERROR, "process packet: AppID %d, UserID %d, type: %d\n", 
			   _message->getAppID(), _message->getUserID(), _message->getMessageType());
		ret = false;
		break;
	}	
	}

	return ret;
}

int service::initGlobalInfo() 
{
	char *ip = CONFIG.get_string_value(CONFIG_SERVER_INT, "ip", NULL);
	int32_t port = CONFIG.get_int_value(CONFIG_SERVER_INT, "port", 0);

	if (ip == NULL || port <= 0 )
    {
		return ERROR;	
	}	
	
	sprintf(global_info.ipPort, "tcp:%s:%d", ip, port);
	
	return SUCCESS;
}

int service::start() 
{
	int ret = SUCCESS;
	char spec[32];
		
	if (initGlobalInfo() != SUCCESS) 
    {
		SYS_LOG(ERROR,"initGlobalInfo() is error");	
		return ERROR;
	}
	
	//监听
	if (transport_.listen(global_info.ipPort, &streamer_, this) == NULL) 
    {
		SYS_LOG(ERROR,"listen address %s", global_info.ipPort) ;
		return ERROR;
	}

	//任务处理线程
	initHandleThreads();

	transport_.start();

	return ret;
}

int service::stop() 
{
	int ret = 0;	

	transport_.stop();

	return ret;	
}

int service::wait() 
{
	int ret = 0;

	taskQueueThread_.wait();
	transport_.wait();

	return ret;		
}	

}//namespace server
 
