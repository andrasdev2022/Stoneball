#include "Bitmap565.h"
#include <string.h>
#include <algorithm>
#include <thread>
#ifndef WIN32
#include <unistd.h>
#endif
#include <future>
#include <vector>
#include <cassert>
#include <iostream>

namespace Bitmap565 {

uint16_t colorTable[64][64];
int colorTableTransparency{-1};

uint16_t make565(int red, int green, int blue) noexcept {
    unsigned short  B =  (blue  >> 3)        & 0x001F;
    unsigned short  G = ((green >> 2) <<  5) & 0x07E0;
    unsigned short  R = ((red   >> 3) << 11) & 0xF800;

    return (unsigned short int) (R | G | B);
}

RGB makeRGB(uint16_t pixel) noexcept {
    unsigned char B = (((pixel)&0x001F) << 3);
    unsigned char G = (((pixel)&0x07E0) >> 3);
    unsigned char R = (((pixel)&0xF800) >> 8);
    return RGB{R, G, B};
}

void copyIntersection(uint16_t* dstBitmap, Rect dstRect, uint16_t dstStride, uint16_t* srcBitmap, Rect srcRect,
                      uint16_t srcStride, bool byMemcpy, int transparency) noexcept {
    //LOGI("DBG dstBitmap: %p, dstRect: (%d, %d, %d, %d), srcBitmap: %p, srcRect: (%d, %d, %d, %d), byMemcpy: %d",
    //    (void*)dstBitmap, dstRect.x, dstRect.y, dstRect.width, dstRect.height,
    //    (void*)srcBitmap, srcRect.x, srcRect.y, srcRect.width, srcRect.height, (int)byMemcpy);

    // Let's find the common part
    int commonLeft = std::max(dstRect.x, srcRect.x);
    int commonTop = std::max(dstRect.y, srcRect.y);
    int commonWidth = std::min(dstRect.x + dstRect.width, srcRect.x + srcRect.width) - commonLeft;
    int commonHeight = std::min(dstRect.y + dstRect.height, srcRect.y + srcRect.height) - commonTop;

    // Conversion from GameMap coordinates to Screen coordinates
    int screenLeft = commonLeft - dstRect.x;
    int screenTop = commonTop - dstRect.y;

    // Conversion from GameMap coordinates to background image coordinates
    int tileLeft = commonLeft - srcRect.x;
    int tileTop = commonTop - srcRect.y;

    //printf("commonLeft: %d, commonTop: %d, commonWidth: %d, commonHeight: %d\n", commonLeft, commonTop, commonWidth, commonHeight);
    //printf("screenLeft: %d, screenTop: %d, tileLeft: %d, tileTop:%d\n", screenLeft, screenTop, tileLeft, tileTop);

    const int transparent = 0;
    // let's copy the common part of the tile image and the screen to the screen
    for(int tileY = 0; tileY < commonHeight; ++tileY) {
        uint16_t* dstStart = dstBitmap + dstStride * (screenTop + tileY) + screenLeft;
        uint16_t* srcStart = srcBitmap + srcStride * (tileTop + tileY) + tileLeft;
        if(byMemcpy) {
            //if(!tileY) {
            //    printf("commonWidth: %d copied\n", commonWidth);
            //}
            // memcpy does not handle the transparent pixels
            memcpy(dstStart, srcStart, commonWidth * sizeof(uint16_t));
        } else if(transparency == 0) {
            for(int tileX = 0; tileX < commonWidth; ++tileX) {
                if(*srcStart != transparent) {
                    *dstStart = *srcStart;
                }
                ++dstStart;
                ++srcStart;
            }
        } else {
            for(int tileX = 0; tileX < commonWidth; ++tileX) {
                if(*srcStart != transparent) {
                    *dstStart = mix565(*srcStart, *dstStart, transparency);
                }
                ++dstStart;
                ++srcStart;
            }
        }
    }
}

void copyIntersectionRarematrix(uint16_t* dstBitmap, Rect dstRect, uint16_t dstStride, uint16_t* srcBitmap, Rect srcRect) noexcept {
    // Let's find the common part
    int commonLeft = std::max(dstRect.x, srcRect.x);
    int commonTop = std::max(dstRect.y, srcRect.y);
    //int commonWidth = std::min(dstRect.x + dstRect.width, srcRect.x + srcRect.width) - commonLeft;
    //int commonHeight = std::min(dstRect.y + dstRect.height, srcRect.y + srcRect.height) - commonTop;

    // Conversion from GameMap coordinates to Screen coordinates
    int screenLeft = commonLeft - dstRect.x;
    int screenTop = commonTop - dstRect.y;

    // Conversion from GameMap coordinates to background image coordinates
    int tileLeft = commonLeft - srcRect.x;
    int tileTop = commonTop - srcRect.y;

    const uint16_t* rareMatrix = srcBitmap;
    uint32_t size = ((uint32_t)rareMatrix[0] | ((uint32_t)rareMatrix[1] << 16));
    const uint16_t*  rareMatrixEnd = rareMatrix + size;
    ++rareMatrix;++rareMatrix;

    int x, y, length, jump, offsetY, offsetX;

    while(rareMatrixEnd > rareMatrix) {
        x = *rareMatrix++;
        y = *rareMatrix++;
        length = *rareMatrix++;
        jump = length;
        offsetY = screenTop + y - tileTop;
        offsetX = screenLeft + x - tileLeft;

        if(offsetY < 0 || offsetY >= dstRect.height) {
            rareMatrix += jump;
            continue;
        }

        if(offsetX < 0) {
            if(offsetX + length >= 0) {
                rareMatrix -= offsetX;
                length += offsetX;
                jump += offsetX;
                offsetX = 0;
            } else {
                rareMatrix += jump;
                continue;
            }
        } else if(offsetX + length > dstRect.width) {
            if(offsetX < dstRect.width) {
                length = dstRect.width - offsetX;
            } else {
                rareMatrix += jump;
                continue;
            }
        }

        uint16_t* dstPos = dstBitmap + dstStride * offsetY + offsetX;
        memcpy(dstPos, rareMatrix, length * sizeof(uint16_t));
        /*
        if(length >= 4) {
            uint64_t* dst = (uint64_t*)dstPos;
            uint64_t* src = (uint64_t*)rareMatrix;
            while(length >= 4) {
                *dst++ = *src++;
                length -= 4;
            }
            if(length > 0) {
                dstPos = (uint16_t*)dst;
                uint16_t* src16 = (uint16_t*)src;
                while(length--) {
                    *dstPos++ = *src16++;
                }
            }
        }
        */
        rareMatrix += jump;
    }
}

uint16_t mix565(uint16_t pixel1, uint16_t pixel2, int transparency) noexcept {
    RGB rgb1 = makeRGB(pixel1);
    RGB rgb2 = makeRGB(pixel2);
    rgb2.red = rgb1.red * transparency / 100 + rgb2.red * (100 - transparency) / 100;
    rgb2.green = rgb1.green * transparency / 100 + rgb2.green * (100 - transparency) / 100;
    rgb2.blue = rgb1.blue * transparency / 100 + rgb2.blue * (100 - transparency) / 100;
    return make565(rgb2.red, rgb2.green, rgb2.blue);
}

void initColorTable(double transparency) noexcept {
    colorTableTransparency = transparency;
    for(int i = 0; i < 64; ++i) {
        for(int j = 0; j < 64; ++j) {
            colorTable[i][j] = (double)i * transparency / 100. + (double)j * (100. - transparency) / 100.;
        }
    }
}

uint16_t fastMix565(uint16_t pixel1, uint16_t pixel2, int transparency) noexcept {
    if(colorTableTransparency != transparency) {
        initColorTable(transparency);
    }

    unsigned int red1   = pixel1 >> 11;
    unsigned int green1 = (pixel1 & 0b0000011111100000) >> 5;
    unsigned int blue1  = pixel1 & 0b0000000000011111;

    unsigned int red2   = pixel2 >> 11;
    unsigned int green2 = (pixel2 & 0b0000011111100000) >> 5;
    unsigned int blue2  = pixel2 & 0b0000000000011111;

    unsigned int red3   = colorTable[red1][red2] << 11;
    unsigned int green3 = colorTable[green1][green2] << 5;
    unsigned int blue3  = colorTable[blue1][blue2];

    return red3 | green3 | blue3;
}


                            //   rrrrrggggggbbbbb
#define MASK_RB       63519 // 0b1111100000011111
#define MASK_G         2016 // 0b0000011111100000
#define MASK_MUL_RB 4065216 // 0b1111100000011111000000
#define MASK_MUL_G   129024 // 0b0000011111100000000000
#define MAX_ALPHA       64 // 6bits+1 with rounding

uint16_t alphablend( uint16_t fg, uint16_t bg, uint16_t alpha ) noexcept {

  // alpha for foreground multiplication
  // convert from 8bit to (6bit+1) with rounding
  // will be in [0..64] inclusive
  alpha = ( alpha + 2 ) >> 2;
  // "beta" for background multiplication; (6bit+1);
  // will be in [0..64] inclusive
  uint16_t beta = MAX_ALPHA - alpha;
  // so (0..64)*alpha + (0..64)*beta always in 0..64

  return (uint16_t)((
            (  ( alpha * (uint32_t)( fg & MASK_RB )
                + beta * (uint32_t)( bg & MASK_RB )
            ) & MASK_MUL_RB )
          |
            (  ( alpha * ( fg & MASK_G )
                + beta * ( bg & MASK_G )
            ) & MASK_MUL_G )
         ) >> 6 );
}

/*
  result masks of multiplications
  uppercase: usable bits of multiplications
  RRRRRrrrrrrBBBBBbbbbbb // 5-5 bits of red+blue
        1111100000011111 // from MASK_RB * 1
  1111100000011111000000 //   to MASK_RB * MAX_ALPHA // 22 bits!


  -----GGGGGGgggggg----- // 6 bits of green
        0000011111100000 // from MASK_G * 1
  0000011111100000000000 //   to MASK_G * MAX_ALPHA
*/

/**
 * Fast RGB565 pixel blending
 * @param fg      The foreground color in uint16_t RGB565 format
 * @param bg      The background color in uint16_t RGB565 format
 * @param alpha   The alpha in range 0-255
 **/
uint16_t alphaBlendRGB565( uint32_t fg, uint32_t bg, uint8_t alpha ) noexcept {
    alpha = ( alpha + 4 ) >> 3;
    bg = (bg | (bg << 16)) & 0b00000111111000001111100000011111;
    fg = (fg | (fg << 16)) & 0b00000111111000001111100000011111;
    uint32_t result = ((((fg - bg) * alpha) >> 5) + bg) & 0b00000111111000001111100000011111;
    return (uint16_t)((result >> 16) | result);
}

} // namespace Bitmap565
