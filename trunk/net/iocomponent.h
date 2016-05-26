#ifndef NET_IOCOMPONENT_H_
#define NET_IOCOMPONENT_H_

namespace net {

class ioComponent {
	friend class transport;
public:
	enum {
		NET_CONNECTING = 1,	
		NET_CONNECTED,
		NET_CLOSED,
		NET_UNCONNECTED	
	};

public:
	ioComponent(transport *owner, Socket *socket);
	
	virtual ~ioComponent();

	virtual bool init(bool isServer = false) = 0;

	virtual void close() {}

	virtual bool handleWriteEvent() = 0;

	virtual bool handleReadEvent() = 0;		

	virtual void checkTimeOut(int64_t now) = 0;

	Socket *getSocket() {
		return _socket;	
	}

	void setSocketEvent(socketEvent *socketEvent) {
		_socketEvent  = socketEvent;	
	}

	void enableWrite(bool writeOn) {
		if (_socketEvent) {
			_socketEvent->setEvent(_socket,true , writeOn);	
		}	
	}

	int addRef() {
		return atomic_add_return(1, &_refcount);	
	}

	void subRef() {
		atomic_dec(&_refcount);	
	}

	int getRef() {
		return atomic_read(&_refcount);	
	}
	
	bool isConnectState() {
		return (_state == NET_CONNECTED || _state == NET_CONNECTING);	
	}	

	int getState() {
		return _state;	
	}

	void setAutoReconn(bool on) {
		_autoReconn = on;	
	}

	bool isAutoReconn() {
		return (_autoReconn && !_isServer);	
	}

	bool isUsed() {
		return _inUsed;	
	}

	void setUsed(bool flag) {
		_inUsed = flag;	
	}

	int64_t getLastUseTime() {
		return _lastUseTime;	
	}

	transport *getOwner();

protected:
	transport *_owner;
	Socket *_socket;
	socketEvent *_socketEvent;
	int _state;
	atomic_t _refcount;
	bool _autoReconn;
	bool _isServer;
	bool _inUsed;
	int64_t _lastUseTime;

private:
	ioComponent *_prev;
	ioComponent *_next;	
};

}//namespace net 

#endif//NET_IOCOMPONENT_H_

