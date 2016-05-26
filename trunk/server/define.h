#ifndef MAASERVER_DEFINE_H_
#define MAASERVER_DEFINE_H_

#include <string.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <string>
#include <map>
#include <vector>
#include <set>
#include <ext/hash_map>

#include "../maanet/maanet.h"
#include "../maasys/maasys.h"

#include "interval.h"
#include "message.h"
#include "func.h"
#include "config.h"
#include "globalInformation.h"
#include "packetstreamer.h"
#include "maaservice.h"
#include "maaserver.h"

#if __WORDSIZE == 32
namespace __gnu_cxx
{
  template<> struct hash<uint64_t>
  {
    uint64_t operator()(uint64_t __x) const
    {
      return __x;
    }
  };
}
#endif

namespace maaserver {
  
class 	message;
class  	configure; 
class  	Func; 
class  	maaService; 
class  	maaPacketStreamer; 
class  	maaServer; 

#define DISALLOW_COPY_AND_ASSIGN(TypeName)\
      TypeName(const TypeName&);\
      void operator=(const TypeName&)

#if __WORDSIZE == 64
  #define PRI64_PREFIX "l"
#else
  #define PRI64_PREFIX "ll"
#endif

#pragma pack(4)  
struct IpAddr
{
  uint32_t ip_;
  int32_t port_;
};
#pragma pack()

enum OperationMode
{
  READ_MODE = 1,
  WRITE_MODE = 2,
  APPEND_MODE = 4,
  UNLINK_MODE = 8,
  NEWBLK_MODE = 16,
  NOLEASE_MODE = 32
};

static const int MAA_SUCCESS = 0;
static const int MAA_ERROR = -1;

static const int32_t INT_SIZE = 4;
static const int32_t INT64_SIZE = 8;
static const int64_t MAA_MALLOC_MAX_SIZE = 0x00A00000;//10M

static const int32_t MAX_RESPONSE_TIME = 30000;

//fsname
static const int32_t FILE_NAME_LEN = 18;
static const int32_t FILE_NAME_EXCEPT_SUFFIX_LEN = 12;
//static const int32_t MAA_FILE_LEN = FILE_NAME_LEN + 1;

static const int32_t ERR_MSG_SIZE = 512;
static const int32_t MAX_FILE_NAME_LEN = 128;
static const int32_t STANDARD_SUFFIX_LEN = 4;

static const int32_t DEFAULT_BLOCK_CACHE_TIME = 5;
static const int32_t DEFAULT_BLOCK_CACHE_ITEMS = 10000;

}//namespace maaserver

#endif//MAASERVER_DEFINE_H_ 

