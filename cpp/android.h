#ifndef __ANDROID_H__
#define __ANDROID_H__

#ifdef ANDROID_NDK
#include <android/log.h>
#include <android/bitmap.h>
#define  LOG_TAG    "libstoneball"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#include "../QtUtil/mock.h"
#endif // __TEST__
#endif //  __ANDROID_H__
