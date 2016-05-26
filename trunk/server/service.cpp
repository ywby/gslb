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

void service::run(maasys::cThread *thread, void *arg) 
{
	std::cout<<"hello"<<endl;	
}	

net::packetHandler::HPRetCode service::handlePacket(maanet::connection *_connection, maanet::packet *_packet) 
{
	if (_connection == NULL || _packet == NULL) 
    {
		return net::packetHandler::FREE_CHANNEL;	
	}	
    
    bool ret = true;
	std::map<int, net::connection *>::iterator it_m; 

    int type =  _packet->_packetHeader.type;	
	if ( type == 1) {  	
		switch (global_info.pattern) {	
		case 1: {
            break;
		}
		case 2: {
			break;
		}
		case 3: {
            break;	
		}
		default : {
			std::cout<<"error"<<endl;	
			break;	
		}	
		}	
	} else if (type  == 2) { 	
		switch (global_info.pattern) {
		case 1: {
			break;	
		}
		case 2: {		
			break;	
		}
		case 3: {
            break;	
		}
		default: {
			std::cout<<"error"<<std::endl;	
			break;	
		}
		}	
	} else if (type  == 3) { 
		switch (global_info.pattern) {
		case 1: {
            break;	
		}
		case 2: {
			break;
		}
		case 3: {
            break;	
		}
		default	: {
			std::cout<<"error"<<std::endl;	
			break;	
		}
		}
	} else { 
		std::cout << "msg error" <<std::endl;	
	}
	
	return maanet::packetHandler::KEEP_CHANNEL;	
}	
	
bool service::handlePacketQueue(net::packet *_packet,void *) 
{
  	bool  ret = true;
	std::map<int, char *>::iterator it_m;
	std::map<char *, maanet::connection *>::iterator it_n;
    
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
			maanet::connection *oldConn =  _message->getConnection();	

			it_m = appidSourceMap.find(_message->getAppID());	
			if (it_m != appidSourceMap.end()) {
				maanet::connection *_conn = transport_.connect(it_m->second, &streamer_, false, false); 
				if (_conn != NULL) {
					_conn->userID = _message->getUserID();	
					_conn->appID = _message->getAppID();	
				    
                    MAASYS_LOG(DEBUG, "create a success connection : addr =%s ", (it_m->second));	

					transport_.socketConnMap.insert(map<int ,maanet::connection *>::value_type(_message->getUserID(),oldConn));	
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
		MAASYS_LOG(ERROR, "process packet: AppID %d, UserID %d, type: %d\n", 
			   _message->getAppID(), _message->getUserID(), _message->getMessageType());
		ret = false;
		break;
	}	
	}

	return ret;
}

int service::initGlobalInfo() 
{
	int  pattern = CONFIG.get_int_value(CONFIG_SERVER_INT, "pattern", 0);
	char *ip = CONFIG.get_string_value(CONFIG_SERVER_INT, "ip", NULL);
	int32_t port = CONFIG.get_int_value(CONFIG_SERVER_INT, "port", 0);

	if (ip == NULL || port <= 0 || (pattern != 1 && pattern != 2 && pattern != 3)){
		return MAA_ERROR;	
	}	
	
	sprintf(global_info.ipPort, "tcp:%s:%d", ip,port);
	global_info.pattern = streamer_.pattern =  pattern;	
	global_info.configServerMap = CONFIG.getConfigMap(CONFIG_SERVER_INT);
	global_info.configAppMap = CONFIG.getConfigMap(CONFIG_APP_INT);
	
	return MAA_SUCCESS;
}

int service::start() 
{
	int ret = MAA_SUCCESS;
	int  appID;
	char spec[32];
		
	if (initGlobalInfo() != MAA_SUCCESS) 
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
 