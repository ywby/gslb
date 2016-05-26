#ifndef MAASERVER_INTERVAL_H_
#define MAASERVER_INTERVAL_H_

#include <string.h>
#include <stdint.h>

#include <string>
#include <map>
#include <vector>
#include <set>
#include <ext/hash_map>


namespace server {

template <typename Type> inline void gDeleteA(Type *&rp) {
    if (rp != NULL)
    {
#ifdef _NO_EXCEPTION
        delete [] rp;
        rp = NULL;
#else
        try
        {
            delete [] rp;
            rp = NULL;
        }
        catch (...)
        {
        }
#endif
        rp = NULL;
    }
}


typedef std::vector<int64_t> VINT64;
typedef std::vector<uint64_t> VUINT64;
typedef std::vector<int32_t> VINT32;
typedef std::vector<uint32_t> VUINT32;
typedef std::vector<int32_t> VINT;
typedef std::vector<uint32_t> VUINT;

static const int32_t PORT_PER_PROCESS = 2;
static const int32_t MAX_DEV_NAME_LEN = 64;
static const int32_t MAX_READ_SIZE = 1048576;

typedef std::map<std::string, std::string> STRING_MAP; 
typedef STRING_MAP::iterator STRING_MAP_ITER;

typedef __gnu_cxx ::hash_map<uint32_t, VINT64> INT_VINT64_MAP; 
typedef INT_VINT64_MAP::iterator INT_VINT64_MAP_ITER;

typedef __gnu_cxx ::hash_map<uint64_t, VINT, __gnu_cxx ::hash<int> > INT64_VINT_MAP; 
typedef INT64_VINT_MAP::iterator INT64_VINT_MAP_ITER;

typedef __gnu_cxx ::hash_map<uint32_t, uint64_t> INT_INT64_MAP; 
typedef INT_INT64_MAP::iterator INT_INT64_MAP_ITER;

typedef __gnu_cxx ::hash_map<uint64_t, uint32_t, __gnu_cxx ::hash<int> > INT64_INT_MAP; 
typedef INT64_INT_MAP::iterator INT64_INT_MAP_ITER;

typedef __gnu_cxx ::hash_map<uint32_t, uint32_t> INT_MAP;
typedef INT_MAP::iterator INT_MAP_ITER;

}//namespace server

#endif//SERVER_INTERVAL_H_
 
