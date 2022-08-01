#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include <stdint.h>
#include "ApplicationExports.h"

#ifdef __cplusplus
extern "C" {
#endif
    // all of your legacy C code here
    void storeBitmap(void* pixels, int bitmapId, int width, int height, int stride, int type);
    void resetFlippedImages();
    uint16_t* bitmapPtr(int bitmapId, bool flipped);
    int bitmapWidth(int bitmapId);
    int bitmapHeight(int bitmapId);
    void calcZoomRate(int actualWidth, int actualHeight, int designWidth, int designHeight);
    double zoomRateWidth();
    double zoomRateHeight();
    double ZRW(double x); // devide by zoomRateWidth()
    double ZRH(double x);  // devide by zoomRateHeight()
    double ZRD(double x);  // for distance calculation, without offsets
    double ZRDI(double x);  // for inverse distance calculation, without offsets
    void onTouchEventNativeDebug(int action, int actionMasked, int actionIndex);
    double now_ms(void);
    void update_now();
    void setScreenSize(int width, int height);
    uint32_t screenWidth();
    uint32_t screenHeight();
    void setScreenPos(int x, int y);
    uint32_t screenPosX();
    uint32_t screenPosY();
    void setControllerZoomRate(double zoomRate);
    double controllerZoomRate();
    const char* getSoundEffect();
    void playSoundEffect(const char* soundeffectName);
    void show_backtrace();
    void setPixelPerfectZoom(bool on);
    void openPrivacyPolicy();
#ifdef __cplusplus
}
#endif


#endif // __GLOBAL_H__
