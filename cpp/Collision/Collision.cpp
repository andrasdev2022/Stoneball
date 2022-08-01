#include "Collision.h"
#include <stdint.h>

// position on the screen/background: (x1, y1) and (x2, y2)
// flipped information is stored in the status
// I have to ask the
bool Collision::detect(int x1, int y1, const Rect& cr1,
                       int x2, int y2, const Rect& cr2,
                       int thresholdX) noexcept {
    //Rect cr1 = findContentRect(width1, height1, framePtr1);
    //Rect cr2 = findContentRect(width2, height2, framePtr2);

    if(cr1.x == -1 && cr1.y == -1 && cr1.width == -1 && cr1.height == -1) return false;
    if(cr2.x == -1 && cr2.y == -1 && cr2.width == -1 && cr2.height == -1) return false;

    return intersectRects(x1 + cr1.x, y1 + cr1.y, cr1.width - thresholdX, cr1.height, x2 + cr2.x, y2 + cr2.y, cr2.width - thresholdX, cr2.height);
}

bool Collision::insideRect(int x1, int y1, int x2, int y2, int width2, int height2) noexcept {
    return  x1 >= x2 && x1 < x2 + width2 && y1 >= y2 && y1 < y2 + height2;
}

bool Collision::insideRect(int x, int y, Rect rect) noexcept {
    return  x >= rect.x && x < rect.x + rect.width && y >= rect.y && y < rect.y + rect.height;
}

bool Collision::intersectRects(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2) noexcept {
    return insideRect(x1, y1, x2, y2, width2, height2) || insideRect(x1 + width1 - 1, y1, x2, y2, width2, height2)
     || insideRect(x1, y1 + height1 - 1, x2, y2, width2, height2) || insideRect(x1 + width1 - 1, y1 + height1 - 1, x2, y2, width2, height2)
     || insideRect(x2, y2, x1, y1, width1, height1) || insideRect(x2 + width2 - 1, y2, x1, y1, width1, height1)
     || insideRect(x2, y2 + height2 - 1, x1, y1, width1, height1) || insideRect(x2 + width2 - 1, y2 + height2 - 1, x1, y1, width1, height1);
}

bool Collision::intersectCircles(int x1, int y1, int r1, int x2, int y2, int r2) noexcept {
    int XX = x1 - x2;
    int YY = y1 - y2;
    int RR = r1 + r2;

    return XX * XX + YY * YY < RR * RR;
}

bool Collision::detectRectCollision(int x1, int y1, Rect cr1,
                                    int x2, int y2, Rect cr2,
                                    int yThreshold, int xThreshold,
                                    int yPercentage /*= 100*/,
                                    int xPercentage /*= 100*/) noexcept {
    // Converting rect to absolute rect on the background
    cr1.x += x1;
    cr1.y += y1;
    cr2.x += x2;
    cr2.y += y2;

    cr1 = updateRectHorizontally(cr1, xPercentage);
    cr1 = updateRectVertically(cr1, yPercentage);
    cr2 = updateRectHorizontally(cr2, xPercentage);
    cr2 = updateRectVertically(cr2, yPercentage);

    return testRects(cr1, cr2, yThreshold, xThreshold);
}

bool Collision::testRects(Rect cr1, Rect cr2, int yThreshold, int xThreshold) noexcept {
    // checks by y
    if(cr1.y + cr1.height < cr2.y - yThreshold) return false;
    if(cr2.y + cr2.height < cr1.y - yThreshold) return false;

    // checks by x
    if(cr1.x + cr1.width < cr2.x - xThreshold) return false;
    if(cr2.x + cr2.width < cr1.x - xThreshold) return false;
    return true;
}

Rect Collision::updateRectVertically(Rect cr, int percentage) noexcept {
    if(percentage != 100 && percentage != -100) {
        if(percentage > 0) {
            cr.height *= percentage/100.;
        } else if(percentage < 0) {
            cr.y += (100. + percentage)/100. * cr.height;
            cr.height *= -percentage/100.;
        }
    }
    return cr;
}

Rect Collision::updateRectHorizontally(Rect cr, int percentage) noexcept {
    if(percentage != 100 && percentage != -100) {
        if(percentage > 0) {
            cr.width *= percentage/100.;
        } else if(percentage < 0) {
            cr.x += (100. + percentage)/100. * cr.width;
            cr.width *= -percentage/100.;
        }
    }
    return cr;
}



/*
 * Poligon collision detection


// Check diagonals of polygon...
            for (int p = 0; p < poly1->p.size(); p++)
            {
                vec2d line_r1s = poly1->pos;
                vec2d line_r1e = poly1->p[p];

                // ...against edges of the other
                for (int q = 0; q < poly2->p.size(); q++)
                {
                    vec2d line_r2s = poly2->p[q];
                    vec2d line_r2e = poly2->p[(q + 1) % poly2->p.size()];

                    // Standard "off the shelf" line segment intersection
                    float h = (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r1e.y) - (line_r1s.x - line_r1e.x) * (line_r2e.y - line_r2s.y);
                    float t1 = ((line_r2s.y - line_r2e.y) * (line_r1s.x - line_r2s.x) + (line_r2e.x - line_r2s.x) * (line_r1s.y - line_r2s.y)) / h;
                    float t2 = ((line_r1s.y - line_r1e.y) * (line_r1s.x - line_r2s.x) + (line_r1e.x - line_r1s.x) * (line_r1s.y - line_r2s.y)) / h;

                    if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
                    {
                        return true;
                    }
                }
            }
*/
