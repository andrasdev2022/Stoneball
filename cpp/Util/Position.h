#ifndef __POSITION_H__
#define __POSITION_H__

#include <math.h>

template<class T>
class Position {
public:
    Position(T x, T y) noexcept : x(x), y(y) {}
    double distance(T x, T y) const noexcept {
        T xs = (this->x - x);
        T ys = (this->y - y);
        return sqrt(xs * xs + ys * ys);
    }

    T x, y;
};

using PositionI = Position<int>;
using PositionD = Position<double>;
#endif // __POSITION_H__
