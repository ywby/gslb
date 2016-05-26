#ifndef SYS_DEFAULT_RUNNABLE_H_
#define SYS_DEFAULT_RUNNABLE_H_

namespace sys {

class defaultRunnable : public runnable {

public:
	defaultRunnable(int threadCount = 1);

	virtual ~defaultRunnable();

	void setThreadCount(int threadCount);

	int start();

	void stop();

	void wait();

protected:    
	cThread *_thread;
	int _threadCount;
	
	bool _stop;
};

}//namespace sys

#endif//SYS_DEFAULT_RUNNABLE_H_

