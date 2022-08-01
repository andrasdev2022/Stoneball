#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>
#include "Math_.h"

template<class T>
class Vector {
public:
    Vector() = default;
    Vector(T x1, T y1, T x2, T y2) noexcept : x(x2 - x1), y(y2 - y1) {}
    Vector(T x1, T y1) noexcept : x(x1), y(y1) {}
    double length() const noexcept {
        return sqrt(x * x + y * y);
    }
    bool isNull() const noexcept {return Math::abs(x) + Math::abs(y) < 1e-8;}
    T x{}, y{};
};

using VectorI = Vector<int>;
using VectorD = Vector<double>;

#endif // __VECTOR_H__
