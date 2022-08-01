#ifndef __COLLISION_H__
#define __COLLISION_H__

#include <stdint.h>
#include <stddef.h>
#include "Rect.h"
#include <memory>

class Collision {
public:
    static bool detect(int x1, int y1, const Rect& cr1,
                        int x2, int y2, const Rect& cr2,
                        int thresholdX) noexcept;
    static bool insideRect(int x1, int y1, int x2, int y2, int width2, int height2) noexcept;
    static bool insideRect(int x, int y, Rect rect) noexcept;
    static bool intersectRects(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) noexcept;
    static bool intersectCircles(int x1, int y1, int r1, int x2, int y2, int r2) noexcept;
    static bool detectRectCollision(int x1, int y1, Rect cr1, int x2, int y2, Rect cr2, int yThreshold, int xThreshold, int yPercentage = 100, int xPercentage = 100) noexcept;
    static bool testRects(Rect cr1, Rect cr2, int yThreshold, int xThreshold) noexcept;
    static Rect updateRectVertically(Rect cr, int percentage) noexcept;
    static Rect updateRectHorizontally(Rect cr, int percentage) noexcept;
};

#endif //  __COLLISION_H__
