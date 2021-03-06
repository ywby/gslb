#ifndef SYS_THREADCOND_H_
#define SYS_THREADCOND_H_

namespace sys {

class threadCond : public threadMutex {

public:
	threadCond() {
    	pthread_cond_init(&_cond, NULL);
	}

	~threadCond() {
        pthread_cond_destroy(&_cond);
    }

	bool wait(int milliseconds = 0) {
		bool ret = true;

		if (milliseconds == 0) { 
			pthread_cond_wait(&_cond, &_mutex);
		} else {
			struct timeval curtime;

			struct timespec abstime;
			gettimeofday(&curtime, NULL);

			int64_t us = (static_cast<int64_t>(curtime.tv_sec) *
					static_cast<int64_t>(1000000) +
					static_cast<int64_t>(curtime.tv_usec) +
					static_cast<int64_t>(milliseconds) *
					static_cast<int64_t>(1000));

			abstime.tv_sec = static_cast<int>(us / static_cast<int64_t>(1000000));
			abstime.tv_nsec = static_cast<int>(us % static_cast<int64_t>(1000000)) * 1000;
			ret = (pthread_cond_timedwait(&_cond, &_mutex, &abstime) == 0);
		}

		return ret;
	}

	void signal() {
		pthread_cond_signal(&_cond);
	}

	void broadcast() {
		pthread_cond_broadcast(&_cond);
	}

private:
    pthread_cond_t _cond;
};

}//namespace sys

#endif//SYS_THREADCOND_H_
