#include "Rect.h"

bool operator==(const Rect& r1, const Rect& r2) noexcept {
    return r1.x == r2.x && r1.y == r2.y && r1.width == r2.width && r1.height == r2.height;
}

std::ostream& operator<<(std::ostream& out, const Rect rect) noexcept {
    out << "Rect(" << rect.x << ", " << rect.y << ", " << rect.width << ", " << rect.height<< ")";
    return out;
}
