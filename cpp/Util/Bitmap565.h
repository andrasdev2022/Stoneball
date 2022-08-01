#ifndef __BITMAP565_H__
#define __BITMAP565_H__

#include <stdint.h>
#include "Rect.h"

namespace Bitmap565 {

    typedef struct {unsigned char red, green, blue;} RGB;

    uint16_t make565(int red, int green, int blue) noexcept;
    RGB makeRGB(uint16_t pixel) noexcept;
    uint16_t mix565(uint16_t pixel1, uint16_t pixel2, int transparency) noexcept;
    uint16_t fastMix565(uint16_t pixel1, uint16_t pixel2, int transparency) noexcept;
    uint16_t alphablend( uint16_t fg, uint16_t bg, uint16_t alpha ) noexcept;
    uint16_t alphaBlendRGB565(uint32_t fg, uint32_t bg, uint8_t alpha) noexcept;

    void copyIntersection(uint16_t* dstBitmap, Rect dstRect, uint16_t dstStride, uint16_t* srcBitmap, Rect srcRect,
                          uint16_t srcStride, bool byMemcpy, int transparency = 0) noexcept;

    void copyIntersectionRarematrix(uint16_t* dstBitmap, Rect dstRect, uint16_t dstStride, uint16_t* srcBitmap, Rect srcRect) noexcept;
} // namespace Bitmap565

#endif // __BITMAP565_H__
