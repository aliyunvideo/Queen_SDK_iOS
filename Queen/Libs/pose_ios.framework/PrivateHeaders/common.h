//
//  commom.h
//  bokeh_ios
//
//  Created by Jiayong Deng on 2021/3/31.
//

#ifndef commom_h
#define commom_h

#include <stdio.h>
#include <algorithm>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <float.h>
#include <time.h>
#include <string.h>
#include <string>
#include <assert.h>
#include <iostream>
#include <stdbool.h>
#include <vector>
#include <map>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <numeric>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;


#if defined(_MSC_VER)
#include <io.h>
#include <direct.h>

#elif defined(__GNUC__)
#include <sys/uio.h>
#include <sys/types.h>
//#include <sys/timeb.h>
#include <sys/time.h>
#include <unistd.h>
#include <dirent.h>
// #include <pthread.h>
#endif


namespace air
{
//#if defined(WIN32) || defined(WIN64) || defined(_MSC_VER)
//
//    typedef unsigned char      air8u;
//    typedef signed char        air8s;
//    typedef unsigned short     air16u;
//    typedef signed short       air16s;
//    typedef unsigned int       air32u;
//    typedef signed int         air32s;
//    typedef unsigned __int64   air64u;
//    typedef __int64            air64s;
//    typedef float              airf32;
//    typedef double             airf64;
//
//#elif defined(__APPLE__) || defined(__GNUC__) || defined(__CC_ARM)
    
    typedef unsigned char      air8u;
    typedef signed char        air8s;
    typedef unsigned short     air16u;
    typedef signed short       air16s;
    typedef unsigned int       air32u;
    typedef signed int         air32s;
    typedef unsigned long long air64u;
    typedef signed long long   air64s;
    typedef float              airf32;
    typedef double             airf64;
    
//#else
//
//#error "data type no definition"
//
//#endif
    
typedef union aai_32suf
{
    air32s i;
    air32u u;
    airf32 f;
} air_32suf;
    
typedef union aai_64suf
{
    air64s i;
    air64u u;
    airf64 f;
} air_64suf;

    
    
    
//#if defined(_MSC_VER)
//#include <windows.h>
//
//    double air_get_current_system_time()
//    {
//    static double freq = -1.0;
//    LARGE_INTEGER big_int;
//    if (freq < 0.0)
//        {
//        if (!QueryPerformanceFrequency(&big_int))
//            return -1.0;
//        freq = (double)big_int.QuadPart;
//        }
//    if (!QueryPerformanceCounter(&big_int))
//        return -1.0;
//    return (big_int.QuadPart * 1000 / freq);
//    }
//
//#elif defined(__GNUC__)
//
//#include <sys/time.h>
//double air_get_current_system_time()
//{
//    struct timeval tv_date;
//    gettimeofday(&tv_date, NULL);
//    return (double)tv_date.tv_sec * 1000 + (double)tv_date.tv_usec * 0.001;
//}
//
//#endif
    


typedef enum
{
    AIR_RET_OK,
    
    AIR_RET_ERR_PARA            = 0x100,
    AIR_RET_ERR_MEM             = 0x101,
    AIR_RET_ERR_NOT_SUPPORT     = 0x102,
    AIR_RET_ERR_OPEN_FILE       = 0x103,
    AIR_RET_ERR_DATA_UNINITED   = 0x104,
    AIR_RET_ERR_UNKNOWN         = 0x105,
    AIR_RET_ERR_END_OF_FILE     = 0x106,
    AIR_RET_ERR_DATA_WAITING    = 0x107,
    AIR_RET_ERR_DATA_NOT_ENOUGH = 0x108,
    AIR_RET_DATA_ALREADY_EXISTS = 0x109,
    AIR_RET_DATA_NOT_FOUND      = 0x110,
    
    AIR_RET_NET_ERR             = 0x111,
    AIR_RER_CGTYPE_UNKNOWN      = 0x112,
    AIR_RER_DATA_FORMAT_UNKNOWN = 0x113,
    AIR_RET_CGRAPH_NOT_DAG      = 0x114,
    AIR_RET_ERR_DATA_INPUT      = 0x115,
    AIR_RET_ERR_DATA_OUTPUT     = 0x116,
    AIR_RET_ERR_NET_CGRAPH      = 0x117,
    AIR_RET_ERR_TENSOR          = 0x118,
    
    AIR_RET_CONF_PARAM_ERR      = 0x201,
    AIR_RET_CONF_PARSE_ERR      = 0x202,
    } AIR_RET;
    
};


#endif /* commom_h */
