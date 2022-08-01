#ifndef __STATICOBJECTS_H__
#define __STATICOBJECTS_H__

#include <stdint.h>
#include <vector>
#include <memory>
#include "Rect.h"
#include <iostream>

class Collision;
class GameObject;

class StaticObjects {
public:
    StaticObjects() noexcept;

    bool applyGravity(GameObject* obj, int yThreshold, int xThreshold, double elapsedTime) noexcept;
    double calcShortestVertDistance(int x, int y, Rect cr, int yThreshold, int xThreshold) noexcept;

    // distance of the closest island under the rect
    int searchMinIslandDistance(const Rect& objectRect, int yThreshold, int xThreshold) noexcept;

    // Islands are NOT included in vertical collision
    void addIslandRect(Rect rect) noexcept;
    void addIslandRects(std::vector<Rect> rects) noexcept;
    std::vector<Rect> islandRects() const noexcept {return islandRects_;}
    void removeIslandRects() noexcept {islandRects_.clear();}

    bool detectRectCollision(const GameObject* obj, int yThreshold, int xThreshold, int yPercentage = 100, int xPercentage = 100) noexcept;

    static const int UNKNOWN_DISTANCE;
private:
    bool mergeRects(Rect& item, Rect rect) noexcept;

    std::vector<Rect> islandRects_;
};

using StaticObjectsUPtr = std::unique_ptr<StaticObjects>;

#endif // __STATICOBJECTS_H__
