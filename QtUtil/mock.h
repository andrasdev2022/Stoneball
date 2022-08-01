#ifndef __MOCK_H__
#define __MOCK_H__

class AndroidBitmapInfo {
public:
    int width;
    int height;
    int stride;
};

#define  LOG_TAG    "libstoneball"
#define  LOGI(...)  __android_log_print(__VA_ARGS__)
#define  LOGE(...)  __android_log_print(__VA_ARGS__)

//#define __VA_ARGS__ va_args
void __android_log_print(const char*,...);

namespace MOCK {
    void logoff();
    void logon();

    enum StoreBitmapType {
        NATIVE_BITMAP_STORE_TYPE_STANDARD,
        NATIVE_BITMAP_STORE_TYPE_RAREMATRIX
    };
}
#endif // __MOCK_H__
