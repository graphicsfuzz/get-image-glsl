#include "timer.h"

/*---------------------------------------------------------------------------*/
// Windows implementation of gettimeofday()

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64

int gettimeofday(struct timeval * tp, struct timezone * tzp) {
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME system_time;
    FILETIME file_time;
    uint64_t time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time = ((uint64_t)file_time.dwLowDateTime );
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

#endif

/*---------------------------------------------------------------------------*/

void startTimer(struct timeval *tv) {
    gettimeofday(tv, 0);
}

/*---------------------------------------------------------------------------*/

long stopTimer(struct timeval *start) {
    struct timeval end;
    gettimeofday(&end, 0);
    time_t elapsed_sec = end.tv_sec - start->tv_sec;
    time_t elapsed_usec = end.tv_usec - start->tv_usec;
    return (long) ((elapsed_sec * 1000000) + elapsed_usec);
}

/*---------------------------------------------------------------------------*/
