#include "sys.h"

namespace sys {

defaultRunnable::defaultRunnable(int threadCount) {
    _stop = false;
    _threadCount = threadCount;
    _thread = NULL;
}

defaultRunnable::~defaultRunnable() {
    if (_thread) {
        delete[] _thread;
        _thread = NULL;
    }
}

void defaultRunnable::setThreadCount(int threadCount) {
    if (_thread != NULL) {
        //SYS_LOG(ERROR, "�Ѿ��������˲��������߳���");
        return;
    }

    _threadCount = threadCount;
}

// start
int defaultRunnable::start() {
    if (_thread != NULL || _threadCount < 1) {
        //SYS_LOG(ERROR, "start failure, _thread: %p, threadCount: %d", _thread, _threadCount);
        return 0;
    }

    _thread = new cThread[_threadCount];
    if (NULL == _thread) {
        //SYS_LOG(ERROR, "create _thread object failed, threadCount: %d", _threadCount);
        return 0;
    }

    int i = 0;
    for (; i<_threadCount; i++) {
        if (!_thread[i].start(this, (void*)((long)i))) {
          return i;
        }
    }

    return i;
}

// stop
void defaultRunnable::stop() {
    _stop = true;
}

// wait
void defaultRunnable::wait() {
    if (_thread != NULL) {
        for (int i=0; i<_threadCount; i++) {
            _thread[i].join();
        }
    }
}

}//namespace sys

