#ifndef __SIZE_H__
#define __SIZE_H__

#include <math.h>
#include <iostream>

template<class T>
class Size {
public:
    Size(T width, T height) noexcept : width(width), height(height) {}

    T width, height;
};

using SizeI = Size<int>;

template<typename T>
std::ostream& operator<<(std::ostream& out, const Size<T> size) noexcept {
    out << "Size(" << size.width << "x" << size.height << ")";
    return out;
}

#endif // __SIZE_H__
