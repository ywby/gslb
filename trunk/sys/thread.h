#ifndef SYS_THREAD_H_
#define SYS_THREAD_H_

#include <linux/unistd.h>

namespace sys {

class cThread {

public:
    cThread() {
        tid = 0;
        pid = 0;
    }

    bool start(runnable *r, void *a) {
        _runnable = r;
        args = a;

        return 0 == pthread_create(&tid, NULL, cThread::hook, this);
    }

    void join() {
        if (tid) {
            pthread_join(tid, NULL);
            tid = 0;
            pid = 0;
        }
    }

    runnable *getRunnable() {
        return _runnable;
    }

    void *getArgs() {
        return args;
    }
    
    int getpid() {
        return pid;
    }

    static void *hook(void *arg) {
        cThread* _thread = (cThread * ) arg;
        _thread->pid = gettid();

        if (_thread->getRunnable()) {
            _thread->getRunnable()->run((cThread * )_thread, _thread->getArgs());
        }

        return (void*) NULL;
    }
    
private:    
    #ifdef _syscall0
        static _syscall0(pid_t,gettid)
    #else
        static pid_t gettid() { return static_cast<pid_t>(syscall(__NR_gettid));}
    #endif

private:
    pthread_t tid;     
    int pid;          
    runnable *_runnable;
    void *args;
};

}//namespace sys

#endif//SYS_THREAD_H_

