#ifndef __MATH_H__
#define __MATH_H__

namespace Math {

template<typename T>
T abs(T x) {
    if(x < 0.) return -x;
    return x;
}

double clamp(double value, double min, double max);
//float Q_rsqrt( float number );

} // namespace Math

#endif // MATH_H
