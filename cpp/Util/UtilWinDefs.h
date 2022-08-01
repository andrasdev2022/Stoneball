#ifndef __UTILWINDEFS_H__
#define __UTILWINDEFS_H__

#ifdef WIN32
#include <winsock2.h> // for struct timeval

    struct timezone {
        int tz_minuteswest;
        int tz_dsttime;
    };

    int gettimeofday(struct timeval* tv, struct timezone* tz);
#endif // WIN32

#endif // __UTILWINDEFS_H__