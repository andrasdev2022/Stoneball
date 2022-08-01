#include "MemCache.h"
#include <string.h>

MemCache::MemCache() {

}

void MemCache::saveWindow(const uint16_t* src, int stride, int width, int height) noexcept {
    makeBuffer(stride, width, height);
    uint16_t* dst = ptr_.get();
    for(uint16_t i = 0; i < height; ++i) {
        memcpy(dst, src, width * sizeof(uint16_t));
        dst += width;
        src += stride;
    }
}

void MemCache::copyWindow(uint16_t* dst) noexcept {
    uint16_t* src = ptr_.get();
    for(uint16_t i = 0; i < height_; ++i) {
        memcpy(dst, src, width_ * sizeof(uint16_t));
        dst += stride_;
        src += width_;
    }
}

void MemCache::makeBuffer(int stride, int width, int height) noexcept {
    width_ = width;
    height_ = height;
    stride_ = stride;

    ptr_ = std::make_unique<uint16_t[]>(width * height);
}
