#include "net.h"

namespace net {

packet::packet() {
	_next = NULL;
	_expireTime = 0;
	atomic_set(&_refcount,0);
	memset(&_packetHeader, 0, sizeof(packetHeader));
}

packet::~packet() {
}

void packet::setExpireTime(int milliseconds) {
    if (milliseconds == 0) {
        milliseconds = 1000*86400;
    }

    _expireTime = sys::timeUtil::getTime() + static_cast<int64_t>(milliseconds) * static_cast<int64_t>(1000);
}

}//namespace net

