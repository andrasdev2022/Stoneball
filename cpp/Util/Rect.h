#ifndef __RECT_H__
#define __RECT_H__

#include <iostream>

class Rect {
public:
    Rect() noexcept : x(-1), y(-1), width(-1), height(-1) {}
    Rect(int x, int y, int width, int height) noexcept : x(x), y(y), width(width), height(height) {}
    ~Rect() noexcept = default;
    Rect(const Rect&) noexcept = default;
    Rect(Rect&&) noexcept = default;
    Rect& operator=(const Rect&) noexcept = default;
    bool isNull() const noexcept {return x == -1 && y == -1 && width == -1 && height == -1;}
    int x, y, width, height;
};

bool operator==(const Rect& r1, const Rect& r2) noexcept;

std::ostream& operator<<(std::ostream& out, const Rect rect) noexcept;

#endif // __RECT_H__
