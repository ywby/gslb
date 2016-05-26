#ifndef SYS_H_
#define SYS_H_

using namespace std;

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <cassert>
#include <time.h>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <vector>
#include <string>
#include "atomic.h"

#ifndef UNUSED
#define UNUSED(v) ((void)(v))
#endif


namespace sys
{
class CLogger;
class netUtil;
class timeUtil;
class cThread;
class threadMutex;
class threadGuard;
class threadCond;
class runnable;
class defaultRunnable;
class sysParam;
class WarningBuffer;
}//namespace sys

#include "log.h"
#include "warningBuffer.h"
#include "netutil.h"
#include "timeutil.h"
#include "runnable.h"
#include "defaultrunnable.h"
#include "thread.h"
#include "threadmutex.h"
#include "parameter.h"
#include "threadcond.h"

#endif//SYS_H_

