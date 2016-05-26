#ifndef SYS_RUNNABLE_H_
#define SYS_RUNNABLE_H_

namespace sys {

class runnable {
public:
	virtual ~runnable()
    {
	}
	
	virtual void run(cThread *thread ,void *arg) = 0;	
};

}//namespace sys

#endif//SYS_RUNNABLE_H_ 
 
