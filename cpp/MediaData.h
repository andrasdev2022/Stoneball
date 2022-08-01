#ifndef __MEDIADATA_H__
#define __MEDIADATA_H__

#include <memory>
#include "Rect.h"

class MediaData {
public:
    enum class Type {
        STANDARD,
        RAREMATRIX
    };

    MediaData(uint16_t* ptr, int width, int height, Rect rect, Type type) noexcept;
    uint16_t* data() const noexcept {return data_.get();}
    int width() const noexcept {return width_;}
    int height() const noexcept {return height_;}
    void setContentRect(Rect rect) noexcept {contentRect_ = rect;}
    const Rect& contentRect() const noexcept {return contentRect_;}
    Type type() const noexcept {return type_;}

private:
    std::unique_ptr<uint16_t[]> data_;
    int width_;
    int height_;
    Rect contentRect_;
    Type type_;
};

MediaData* bitmapData(int type, bool flipped) noexcept;
double operator"" _ZRW(long double x) noexcept;
double operator"" _ZRH(long double y) noexcept;
double operator"" _ZRD(long double d) noexcept;

int operator"" _ZRW(unsigned long long int x) noexcept;
int operator"" _ZRH(unsigned long long int y) noexcept;
int operator"" _ZRD(unsigned long long int d) noexcept;

#endif // __MEDIADATA_H__
