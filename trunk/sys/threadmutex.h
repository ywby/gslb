#ifndef SYS_THREADMUTEX_H_
#define SYS_THREADMUTEX_H_

namespace sys {

class threadMutex {

public:
	threadMutex() {
		const int ret = pthread_mutex_init(&_mutex,NULL);
		assert( ret == 0);	
	}

	~threadMutex(){
		pthread_mutex_destroy(&_mutex);	
	}

	void lock() {
		pthread_mutex_lock(&_mutex);
	}

	int tryLock() {
		return pthread_mutex_trylock(&_mutex);	
	}

	void unlock() {
		pthread_mutex_unlock(&_mutex);	
	}

protected:
	pthread_mutex_t _mutex;	
};

class threadGuard
{
public:
	threadGuard(threadMutex *mutex) {
  		_mutex = NULL;
    	if (mutex) {
    		_mutex = mutex;
    		_mutex->lock();
    	}
    }
    
	~threadGuard() {
    	if (_mutex) {
        	_mutex->unlock();
    	}
	}

private:
    threadMutex *_mutex;
};

}//namespace sys

#endif//SYS_THREADMUTEX_H_

