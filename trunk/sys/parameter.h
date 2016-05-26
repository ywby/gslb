#ifndef SYS_PARAMTER_H_
#define SYS_PARAMTER_H_

namespace sys {

class sysParam {

public:
	struct sysConf {
		int32_t port;
	};

	const sysConf& sysconf() const {
		return sys_conf;	
	}
	
	static sysParam& instance() {
		return _instance;	
	}

private:
	static sysParam _instance;
	sysConf sys_conf;	
};

#define SYSPARAM_CONF sysParam::instance().sysconf();

}//namespace sys

#endif//SYS_PARAMTER_H_


