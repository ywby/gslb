#include "net.h"

namespace net {

transport::transport() {
	_stop = false;

	_iocListHead = _iocListTail = NULL;
	_delListHead = _delListTail = NULL;

	_iocListChanged = false;
	_iocListCount = 0;
}

transport::~transport(){
	destroy();
}

bool transport::start() {
	signal(SIGPIPE, SIG_IGN);
	_readWriteThread.start(this, &_socketEvent);
	_timeoutThread.start(this, NULL);
	return true;
}

bool transport::stop() {
	_stop = true;
	return true;
}

bool transport::wait() {
	_readWriteThread.join();
	_timeoutThread.join();
	destroy();
	return true;
}


int transport::splitAddr(char *src, char **args,int cnt) 
{
	int index = 0;
	char *strTmp = src;
	
	while (*src) 
    {
		if (*src == ':') 
        {
			*src = '\0';	
			args[index++] = strTmp;	
		
			if (index >= cnt) 
            {	
				return index;
			}
			strTmp = src + 1;	
		}
		src++;	
	}

	if (index < cnt) 
    {	
		args[index++] = strTmp;
	}	
	
	return index;			
}

void transport::removeComponent(ioComponent *ioc) {
	assert(ioc != NULL);

	sys::threadGuard guard(&_iocMutex);
	ioc->close();

	if (ioc->isAutoReconn()) { // 需要重连, 不从iocomponents去掉
		return;
	}

	if (ioc->isUsed() == false) { // 不在iocList中
		return;
	}

	// 从_iocList删除
	if (ioc == _iocListHead) { // head
		_iocListHead = ioc->_next;
	}
	if (ioc == _iocListTail) { // tail
		_iocListTail = ioc->_prev;
	}

	if (ioc->_prev != NULL)
		ioc->_prev->_next = ioc->_next;
	if (ioc->_next != NULL)
		ioc->_next->_prev = ioc->_prev;

	// 加入到_delList
	ioc->_prev = _delListTail;
	ioc->_next = NULL;

	if (_delListTail == NULL) {
		_delListHead = ioc;
	} else {
		_delListTail->_next = ioc;
	}
	_delListTail = ioc;

	// 引用计数减一
	ioc->setUsed(false);
	_iocListChanged = true;
	_iocListCount --;

	//SYS_LOG(INFO, "RMIOC, %s IOCount:%d, IOC:%p\n", 
    //		        ioc->getSocket()->getAddr().c_str(), _iocListCount, ioc);
}

ioComponent *transport::listen(const char *spec, packetStreamer *streamer, serverAdapter *_serverAdapter) 
{
	char strTmp[512];
	char *args[32];
	char *host;
	int port;

	strncpy(strTmp, spec, 512);
	strTmp[511] = '\0';	

	if (splitAddr(strTmp,args,32) != 3) 
    {
		return NULL;	
	}

	if (strcasecmp(args[0],"tcp") == 0) 
    {
		host = args[1];
		port = atoi(args[2]);	
	
		serverSocket *socket = new serverSocket();
		if (!socket->setAddress(host,port)) 
        {
			delete socket;
			return NULL;	
		}

		tcpAccepter *accepter = new tcpAccepter(this ,socket, streamer,_serverAdapter);	
		if(!accepter->init())
        {
			delete accepter;
			return NULL;	
		}

		addComponent(accepter, true, false);

		return accepter;	
	} 
    else if (strcasecmp(args[0], "udp") == 0) 
    {   
	}

	return NULL;
}

void transport::addComponent(ioComponent *ioc, bool readOn, bool writeOn) 
{
	_iocMutex.lock();
	if (ioc->isUsed()) {
		_iocMutex.unlock();
		return;	
	}

	ioc->_prev = _iocListTail;
	ioc->_next = NULL;

	if (_iocListTail == NULL) {
		_iocListHead = ioc;	
	} else {
		_iocListTail->_next = ioc;	
	}
	_iocListTail = ioc;

	ioc->setUsed(true);
	_iocListChanged = true;
	_iocListCount++;
	_iocMutex.unlock();
	
	Socket *socket = ioc->getSocket();
	ioc->setSocketEvent(&_socketEvent);
	_socketEvent.addEvent(socket,readOn,writeOn);
}

void transport::run(sys::cThread *thread, void *arg) {
	if (thread == &_timeoutThread) {
		timeoutLoop();	
	} else {
		eventLoop((socketEvent*)arg);	
	}
}

void transport::eventLoop(socketEvent *_socketEvent) {
	ioEvent events[MAX_SOCKET_EVENTS];

	while (!_stop) {
		int cnt = _socketEvent->getEvents(1000, events, MAX_SOCKET_EVENTS);
		if (cnt < 0) {
			//sys.log(错误)	
		}

		for (int i = 0; i < cnt; i++ ) {
			ioComponent *ioc = events[i]._ioc;
			if (ioc == NULL) {
				continue;	
			}

			if (events[i]._errorEvent) {
				//SYS_LOG(ERROR,"removecomponent :socket =%d",  ioc->_socket->_socketHandle);	
				removeComponent(ioc);
				continue;
			}	

			ioc->addRef();

			bool ret = true;

			if(events[i]._readEvent) {
				//SYS_LOG(DEBUG,"readEvent : socket =%d", ioc->_socket->_socketHandle);	
				ret = ioc->handleReadEvent();	
			}

			if (ret && events[i]._writeEvent) {
				//SYS_LOG(DEBUG,"writeEvent: socket =%d",  ioc->_socket->_socketHandle);	
				ret = ioc->handleWriteEvent();	
			}

			ioc->subRef();	
			
			if (!ret) {
	      //SYS_LOG(ERROR,"removecomponent :socket=%d", ioc->_socket->_socketHandle);	
				removeComponent(ioc);	
			}	
		}	
	}	
}

void transport::timeoutLoop() {
	ioComponent *mydelHead = NULL;
	ioComponent *mydelTail = NULL;

	std::vector<ioComponent*> mylist;

	while (!_stop) {
		_iocMutex.lock();
		if (_iocListChanged) {
			mylist.clear();

			ioComponent *iocList = _iocListHead;
			while (iocList) {
				mylist.push_back(iocList);
				iocList = iocList->_next;
			}

			_iocListChanged = false;
		}
	
		if (_delListHead != NULL && _delListTail != NULL) {
			if (mydelTail == NULL) {
				mydelHead = _delListHead;
			} else {
				mydelTail->_next = _delListHead;
				_delListHead->_prev = mydelTail;
			}

			mydelTail = _delListTail;
			_delListHead = _delListTail = NULL;
		}
		_iocMutex.unlock();

		for (int i=0; i<(int)mylist.size(); i++) {
			ioComponent *ioc = mylist[i];
			ioc->checkTimeOut(sys::timeUtil::getTime());
		}

		// 删除掉
		ioComponent *tmpList = mydelHead;
		int64_t nowTime = sys::timeUtil::getTime() - static_cast<int64_t>(900000000); // 15min

		while (tmpList) {
			if (tmpList->getRef() <= 0) {
				tmpList->subRef();
			}

			if (tmpList->getRef() <= -10 || tmpList->getLastUseTime() < nowTime) {
				// 从链中删除
				if (tmpList == mydelHead) { // head
					mydelHead = tmpList->_next;
				}
				if (tmpList == mydelTail) { // tail
					mydelTail = tmpList->_prev;
				}

				if (tmpList->_prev != NULL)
					tmpList->_prev->_next = tmpList->_next;
				if (tmpList->_next != NULL)
					tmpList->_next->_prev = tmpList->_prev;

				ioComponent *ioc = tmpList;
				tmpList = tmpList->_next;
				//sys.log(INFO, "DELIOC, %s, IOCount:%d, IOC:%p\n",
						//ioc->getSocket()->getAddr().c_str(), _iocListCount, ioc);

				delete ioc;
			} else {
				tmpList = tmpList->_next;
			}
		}

		usleep(500000);  // 最小间隔100ms
	}

	// 写回到_delList上,让destroy销毁
	_iocMutex.lock();
	if (mydelHead != NULL) {
		if (_delListTail == NULL) {
			_delListHead = mydelHead;
		} else {
			_delListTail->_next = mydelHead;
			mydelHead->_prev = _delListTail;
		}

		_delListTail = mydelTail;
	}
	_iocMutex.unlock();
}

void transport::destroy() {
	sys::threadGuard guard(&_iocMutex);

	ioComponent *list, *ioc;
	// 删除iocList
	list = _iocListHead;

	while (list) {
		ioc = list;
		list = list->_next;
		_iocListCount --;

		//sys.log(INFO, "DELIOC, IOCount:%d, IOC:%p\n",_iocListCount, ioc);
		delete ioc;
	}

	_iocListHead = _iocListTail = NULL;
	_iocListCount = 0;

	// 删除delList
	list = _delListHead;

	while (list) {
		ioc = list;
		assert(ioc != NULL);

		list = list->_next;

		//sys.log(INFO, "DELIOC, IOCount:%d, IOC:%p\n",_iocListCount, ioc);
		delete ioc;
	}

	_delListHead = _delListTail = NULL;
}

bool* transport::getStop() {
	return &_stop;
}

connection *transport::connect(const char *spec, packetStreamer *streamer, bool autoReconn, bool appTcp) {
	char tmp[1024];
	char *args[32];

	strncpy(tmp, spec, 1024);
	tmp[1023] = '\0';

	if (splitAddr(tmp, args, 32) != 3) {
		return NULL;
	}

	if (strcasecmp(args[0], "tcp") == 0) {
		char *host = args[1];
		int port = atoi(args[2]);

		Socket *socket = new Socket();
		if (!socket->setAddress(host, port)) {
			delete socket;

			//sys.log(ERROR, "设置setAddress错误: %s:%d, %s", host, port, spec);
			return NULL;
		}

		tcpComponent *component = new tcpComponent(this, socket, streamer,NULL, false, appTcp);
		
		//重连标记	
		component->setAutoReconn(autoReconn);
		if (!component->init()) {
			delete component;

			//sys.log(ERROR, "初始化失败TCPComponent: %s:%d", host, port);
			std::cout<<"delete component"<<std::endl;	

			return NULL;
		}

		addComponent(component, true, true);
		component->addRef();

		return component->getConnection();

	} else if (strcasecmp(args[0], "udp") == 0) {
		;
	}

	return NULL;
}

bool transport::disconnect(connection *conn) {
	ioComponent *ioc = NULL;

	if (conn == NULL || (ioc = conn->getIoComponent()) == NULL) {
		return false;
	}

	ioc->setAutoReconn(false);
	ioc->subRef();

	if (ioc->_socket) {
		ioc->_socket->shutdown();
	}
	
	return true;
}

}//namespace net

