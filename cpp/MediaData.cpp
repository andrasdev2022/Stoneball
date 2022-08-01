#include "MediaData.h"
#include "stoneball.h"
#include <array>
#include <vector>
#include <string>
#include <math.h>
#include "BitmapIds.h"
#include "Defines.h"
#include "CommonUtil.h"

#ifndef ANDROID_NDK
#include "DesktopBitmapIds.h"
#endif

MediaData::MediaData(uint16_t* ptr, int width, int height, Rect rect, Type type) noexcept
 : data_(std::unique_ptr<uint16_t[]>(ptr))
 , width_(width)
 , height_(height)
 , contentRect_(rect)
 , type_(type)
{
}

namespace {
#ifdef ANDROID_NDK
THREAD_LOCAL std::array<std::unique_ptr<MediaData>, LAST_ONE> mediaData;
THREAD_LOCAL std::array<std::unique_ptr<MediaData>, LAST_ONE> flippedMediaData;
#else
#include "DesktopBitmapIds.h"
THREAD_LOCAL std::vector<std::unique_ptr<MediaData>> mediaData(DesktopBitmapIds::last_one());
THREAD_LOCAL std::vector<std::unique_ptr<MediaData>> flippedMediaData(DesktopBitmapIds::last_one());
#endif
THREAD_LOCAL double withZoomRate_ = 1., heightZoomRate_ = 1.;
THREAD_LOCAL uint32_t screenWidth_ = 1920;
THREAD_LOCAL uint32_t screenHeight_ = 1080;
THREAD_LOCAL bool pixelPerfectZoom_ = false;
THREAD_LOCAL uint32_t screenPosX_ =  0;
THREAD_LOCAL uint32_t screenPosY_ =  0;
THREAD_LOCAL double controllerZoomRate_ = 1.;
THREAD_LOCAL uint32_t zoomRateOffsetX_ = 0;
THREAD_LOCAL uint32_t zoomRateOffsetY_ = 0;
constexpr int transparentColor_ = 0;
}

void updateContentRect(Rect& contentRect, int x, int y) noexcept {
    if(contentRect.width == -1) {
        contentRect.x = x; // minX
        contentRect.width = x; // maxX
    } else {
        if(contentRect.x > x) contentRect.x = x; // minX
        if(contentRect.width < x) contentRect.width = x; // maxX
    }
    if(contentRect.height == -1) {
        contentRect.y = y; // minY
        contentRect.height = y; // maxY
    } else {
        if(contentRect.y > y) contentRect.y = y; // minY
        if(contentRect.height < y) contentRect.height = y; // maxY
    }
}

void convertContentRect(Rect& contentRect) noexcept {
    // comverting maxX, maxY to width, height
    if(contentRect.width != -1) {
        contentRect.width = contentRect.width - contentRect.x + 1;
    }
    if(contentRect.height != -1) {
        contentRect.height = contentRect.height - contentRect.y + 1;
    }
}

void storeBitmapStandard(void* pixels, const int bitmapId, int width, int height, int stride) noexcept {
    uint16_t* bitmapPtr = new uint16_t[width * height];
    uint16_t* bg = bitmapPtr;
    void* pixelsCP = pixels;
    Rect contentRect;
    for(int  yy = 0; yy < height; yy++) {
        uint16_t* line = (uint16_t*)pixelsCP;
        for(int xx = 0; xx < width; xx++) {
            *bg++ = line[xx];
            if(line[xx] != transparentColor_){ // if not transparent
                updateContentRect(contentRect, xx, yy);
            }
        }
        //line += backgroundInfo.width;
        pixelsCP = (char*)pixelsCP + stride;
    }
    convertContentRect(contentRect);
#ifdef __SERVERAPP__
    mediaData[bitmapId] = std::make_unique<MediaData>(nullptr, width, height, contentRect, MediaData::Type::STANDARD);
    delete [] bitmapPtr;
#else
    mediaData[bitmapId] = std::make_unique<MediaData>(bitmapPtr, width, height, contentRect, MediaData::Type::STANDARD);
#endif
}

void generateFlippedBitmapStandard(int bitmapId, MediaData* imgData) {
    Rect contentRect;
    uint16_t* bitmapPtr = new uint16_t[imgData->width() * imgData->height()];
    uint16_t* bg = bitmapPtr;
    uint16_t* pixelsCP = imgData->data();
    for(int yy = 0; yy < imgData->height(); yy++) {
        uint16_t* line = (uint16_t*)pixelsCP;
        for(int xx = 0; xx < imgData->width(); xx++) {
            *bg++ = line[imgData->width() - xx - 1];
            if(line[imgData->width() - xx - 1] != transparentColor_) { // if not transparent
                updateContentRect(contentRect, xx, yy);
            }
        }
        pixelsCP += imgData->width();
    }
    convertContentRect(contentRect);
#ifdef __SERVERAPP__
    flippedMediaData[bitmapId] = std::make_unique<MediaData>(nullptr, imgData->width(), imgData->height(), contentRect, MediaData::Type::STANDARD);
    delete [] bitmapPtr;
#else
    flippedMediaData[bitmapId] = std::make_unique<MediaData>(bitmapPtr, imgData->width(), imgData->height(), contentRect, MediaData::Type::STANDARD);
#endif
}

void generateFlippedBitmapRareMatrix(int bitmapId, MediaData* imgData) {
    Rect contentRect = imgData->contentRect();
    contentRect.x = imgData->width() - imgData->contentRect().x - imgData->contentRect().width;

#ifndef __SERVERAPP__
    uint16_t* rareMatrix = imgData->data();
    uint32_t size = ((uint32_t)rareMatrix[0] | ((uint32_t)rareMatrix[1] << 16));
    uint16_t* newRareMatrix = new uint16_t[size];
    uint16_t* newRareMatrixO = newRareMatrix;

    newRareMatrix[0] = rareMatrix[0]; // this will be the size lower 16 bits
    newRareMatrix[1] = rareMatrix[1]; // this will be the size higher 16 bits
    const uint16_t* rareMatrixEnd = rareMatrix + size;

    rareMatrix += 2;
    newRareMatrix += 2;
    while(rareMatrixEnd > rareMatrix) {
        // conversion of x position of the next block
        uint16_t* xpos = newRareMatrix;
        uint16_t xposValue = *rareMatrix;
        ++rareMatrix; ++newRareMatrix;

        // y position of the next block
        *newRareMatrix++ = *rareMatrix++;

        uint16_t length = *rareMatrix++; // length of the block
        *newRareMatrix++ = length;

        *xpos = imgData->width() - xposValue - length;
        // copy of the block
        for(uint16_t i = 0; i < length; ++i) {
           newRareMatrix[i] = rareMatrix[length - i - 1];
        }

        rareMatrix += length;
        newRareMatrix += length;
    }
    flippedMediaData[bitmapId] = std::make_unique<MediaData>(newRareMatrixO, imgData->width(), imgData->height(), contentRect, MediaData::Type::RAREMATRIX);
#else
    flippedMediaData[bitmapId] = std::make_unique<MediaData>(nullptr, imgData->width(), imgData->height(), contentRect, MediaData::Type::RAREMATRIX);
#endif
}

void generateFlippedBitmap(int bitmapId) {
    MediaData* imgData = bitmapData(bitmapId, false);
    if(imgData->type() == MediaData::Type::STANDARD) {
        generateFlippedBitmapStandard(bitmapId, imgData);
    } else {
        generateFlippedBitmapRareMatrix(bitmapId, imgData);
    }
}

void storeBitmapRareMatrix(void* pixels, int bitmapId, int width, int height, int stride) noexcept {
    //Unused(0, pixels, bitmapId, width, height, stride);
    //LOGI("DBG storeBitmapRareMatrix begin");

    std::vector<uint16_t> bitmapV;
    bitmapV.push_back(0); // this will be the size lower 16 bits
    bitmapV.push_back(0); // this will be the size higher 16 bits

    void* pixelsCP = pixels;
    Rect contentRect;

    int lengthPos = 0;

    for(int yy = 0; yy < height; yy++) {
        uint16_t* line = (uint16_t*)pixelsCP;
        bool blockStarted = false;
        for(int xx = 0; xx < width; xx++) {
            uint16_t pixel = line[xx];
            if(pixel != transparentColor_) { // if the pixel isn't transparent
                updateContentRect(contentRect, xx, yy);
                if(blockStarted) {
                    bitmapV.push_back(pixel);
                    ++bitmapV[lengthPos];
                } else {
                    //LOGI("DBG storeBitmapRareMatrix xx: %d, yy: %d", xx, yy);
                    bitmapV.push_back(xx);
                    bitmapV.push_back(yy);
                    lengthPos = bitmapV.size();
                    bitmapV.push_back(1);
                    bitmapV.push_back(pixel);
                    blockStarted = true;
                }
            } else {
                blockStarted = false;
            }
        }
        pixelsCP = (char*)pixelsCP + stride;
    }
    convertContentRect(contentRect);
    bitmapV[0] = bitmapV.size() & 0xffff;
    bitmapV[1] = bitmapV.size() >> 16;

#ifdef __SERVERAPP__
    mediaData[bitmapId] = std::make_unique<MediaData>(nullptr, width, height, contentRect, MediaData::Type::RAREMATRIX);
#else
    uint16_t* bitmapPtr = new uint16_t[bitmapV.size()];
    std::copy(bitmapV.begin(), bitmapV.end(), bitmapPtr);
    mediaData[bitmapId] = std::make_unique<MediaData>(bitmapPtr, width, height, contentRect, MediaData::Type::RAREMATRIX);
#endif
}


MediaData* bitmapData(int bitmapId, bool flipped) noexcept {
    if(flipped) {
        if(!flippedMediaData[bitmapId]) {
            generateFlippedBitmap(bitmapId);
        }
        return flippedMediaData[bitmapId].get();
    }
    return mediaData[bitmapId].get();
}

extern "C" {

// Global.h
void storeBitmap(void* pixels, int bitmapId, int width, int height, int stride, int type) {
    switch(type) {
        case (int)MediaData::Type::STANDARD:
            storeBitmapStandard(pixels, bitmapId, width, height, stride);
        break;
        case (int)MediaData::Type::RAREMATRIX:
            storeBitmapRareMatrix(pixels, bitmapId, width, height, stride);
        break;
        default:
            debug("ERROR: storeBitmap: Illegal MediaData::Type");
            throw std::runtime_error("ERROR: storeBitmap: Illegal MediaData::Type");
    }
}

// Global.h
void resetFlippedImages() {
    for(auto& x : flippedMediaData) x = nullptr;
}

// Global.h
uint16_t* bitmapPtr(int bitmapId, bool flipped) {
    if(flipped) {
        if(!flippedMediaData[bitmapId]) {
            generateFlippedBitmap(bitmapId);
        }
        return flippedMediaData[bitmapId]->data();
    }
    return mediaData[bitmapId]->data();
}

// Global.h
int bitmapWidth(int bitmapId) {
    return mediaData[bitmapId]->width();
}

// Global.h
int bitmapHeight(int bitmapId) {
    return mediaData[bitmapId]->height();
}

// Global.h
void calcZoomRate(int actualWidth, int actualHeight, int designWidth, int designHeight) {
    withZoomRate_ = (double)actualWidth / designWidth;
    heightZoomRate_ = (double)actualHeight / designHeight;
    if(pixelPerfectZoom_) {
        withZoomRate_ = floor(withZoomRate_);
        heightZoomRate_ = floor(heightZoomRate_);
        //std::cout << withZoomRate_ << ", " << heightZoomRate_ << "\n";
    }
    if(withZoomRate_ > heightZoomRate_) {
        zoomRateOffsetX_ = (actualWidth - heightZoomRate_ * designWidth) / 2;
        zoomRateOffsetY_ = (actualHeight - heightZoomRate_ * designHeight) / 2;
    } else {
        zoomRateOffsetX_ = (actualWidth - withZoomRate_ * designWidth) / 2;
        zoomRateOffsetY_ = (actualHeight - withZoomRate_ * designHeight) / 2;
    }
}

// Global.h
double zoomRateWidth() {
    return withZoomRate_;
}

// Global.h
double zoomRateHeight() {
    return heightZoomRate_;
}

// Global.h
void setScreenSize(int width, int height) {
    screenWidth_ = width;
    screenHeight_ = height;
}

// Global.h
uint32_t screenWidth() {
    return screenWidth_;
}

// Global.h
uint32_t screenHeight() {
    return screenHeight_;
}

// Global.h
void setScreenPos(int x, int y) {
    screenPosX_ = x;
    screenPosY_ = y;
}

// Global.h
uint32_t screenPosX() {
    return screenPosX_;
}

// Global.h
uint32_t screenPosY() {
    return screenPosY_;
}

// Global.h
// x position calculation for mobile from plan position
double ZRW(double x) {
    return x * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetX_;
}

// Global.h
// y position calculation for mobile from plan position
double ZRH(double y) {
    return y * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetY_;
}

// Global.h
// distance calculation
double ZRD(double d) {
    return d * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight());
}

// Global.h
// inverse distance calculation
double ZRDI(double d) {
    return d / (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight());
}

// Global.h
void setControllerZoomRate(double zoomRate) {
    controllerZoomRate_ = zoomRate;
}

// Global.h
double controllerZoomRate() {
    return controllerZoomRate_;
}

// Global.h
void setPixelPerfectZoom(bool on) {
    pixelPerfectZoom_ = on;
}

} // extern "C"


// x position calculation for mobile from plan position
double operator"" _ZRW(long double x) noexcept {
    return x * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetX_;
}

// y position calculation for mobile from plan position
double operator"" _ZRH(long double y) noexcept {
    return y * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetY_;
}

double operator"" _ZRD(long double d) noexcept {
    return d * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight());
}

// x position calculation for mobile from plan position
int operator"" _ZRW(unsigned long long int x) noexcept {
    return x * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetX_;
}

// y position calculation for mobile from plan position
int operator"" _ZRH(unsigned long long int y) noexcept {
    return y * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight()) + zoomRateOffsetY_;
}

int operator"" _ZRD(unsigned long long int d) noexcept {
    return d * (zoomRateHeight() > zoomRateWidth() ? zoomRateWidth() : zoomRateHeight());
}

