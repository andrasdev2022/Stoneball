#ifndef __DEFINES_H__
#define __DEFINES_H__

//#ifndef ANDROID_NDK
//#define THREAD_LOCAL thread_local
//#else
#define THREAD_LOCAL
//#endif


#if !defined(ANDROID_NDK)
class AndroidBitmapInfo;
#else
#include <android/bitmap.h>
#endif

#endif // __DEFINES_H__
