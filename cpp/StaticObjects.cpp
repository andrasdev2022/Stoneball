#include "StaticObjects.h"
#include "BitmapIds.h"
#include "stoneball.h"
#include "GameObject.h"
#include "MediaData.h"
#include "Collision.h"
#include "Global.h"

const int StaticObjects::UNKNOWN_DISTANCE = 1e+7;

StaticObjects::StaticObjects() noexcept
{
    //islandRects_.push_back(Rect(0, 792, 431, 1));
}

double StaticObjects::calcShortestVertDistance(int x, int y, Rect cr, int yThreshold, int xThreshold) noexcept {
    //Rect cr = collision_->findContentRect(width, height, framePtr);

    // Converting rect to absolute rect on the background
    cr.x += x;
    cr.y += y;

    return searchMinIslandDistance(cr, yThreshold, xThreshold);
}

// distance of the closest island UNDER the rect
int StaticObjects::searchMinIslandDistance(const Rect& contentRect, int yThreshold, int xThreshold) noexcept {
    // The frame in an animation can end on a different y position. This threshold handles it.
    //static const int threshold = 10;
    int distance = UNKNOWN_DISTANCE;
    int startY = contentRect.y + contentRect.height - 1;
    for(size_t i = 0; i < islandRects_.size(); ++i) {
        const Rect& rect = islandRects_[i];
        if(rect.x + rect.width < contentRect.x  + xThreshold || contentRect.x + contentRect.width < rect.x + xThreshold) continue;
        if(rect.y >= startY - yThreshold && distance >= rect.y - startY - yThreshold) {
            distance = rect.y - startY;
        }
    }
    return distance;
}

bool StaticObjects::detectRectCollision(const GameObject* obj, int yThreshold, int xThreshold, int yPercentage /*= 100*/, int xPercentage /*= 100*/) noexcept {
    if(!obj->collisionData()) return false;
    Rect cr = obj->collisionData()->rect();

    // Converting rect to absolute rect on the background
    cr.x += obj->x();
    cr.y += obj->y();

    cr = Collision::updateRectHorizontally(cr, xPercentage);
    cr = Collision::updateRectVertically(cr, yPercentage);

    for(size_t i = 0; i < islandRects_.size(); ++i) {
        Rect rect = islandRects_[i];
        rect = Collision::updateRectHorizontally(rect, xPercentage);
        rect = Collision::updateRectVertically(rect, yPercentage);
        if(!Collision::testRects(cr, rect, yThreshold, xThreshold)) continue;
        return true;
    }
    return false;
}

/*
Return value true if it's on the ground, otherwise false
*/
bool StaticObjects::applyGravity(GameObject* obj, int yThreshold, int xThreshold, double elapsedTime) noexcept {
    const int dropStep = screenHeight() / 40;
    int step = dropStep * elapsedTime / 1000. * 60.;
    //int step = dropStep;Unused(0, elapsedTime);
    int distance = calcShortestVertDistance(obj->x(), obj->y(), obj->frameData()->contentRect(), yThreshold, xThreshold);
    if(distance > 1) {
        //LOGI("DBG distance: %d, hero y: %f", distance, hero->y());
        bool collision = distance <= step;
        if(!collision) distance = step;
        obj->yCorrectionByGravity(obj->y() + distance - 1, collision);
        return false;
    }
    return true;
}

void StaticObjects::addIslandRect(Rect rect) noexcept {
    // merge rect horizontally, if it is possible
    for(size_t i = 0; i < islandRects_.size(); ++i) {
        Rect& item = islandRects_[i];
        if(mergeRects(item, rect)) return;
    }
    islandRects_.push_back(rect);
}

void StaticObjects::addIslandRects(std::vector<Rect> rects) noexcept {
    //std::copy(rects.begin(), rects.end(), std::back_inserter(islandRects_));
    for(size_t i = 0; i < rects.size(); ++i) {
        addIslandRect(rects[i]);
    }
}

bool StaticObjects::mergeRects(Rect& item, Rect rect) noexcept {
    if(item.y != rect.y || item.height != rect.height) return false;
    if(rect.x + rect.width == item.x) {
        item.width += rect.width;
        item.x = rect.x;
        return true;
    } else if(item.x + item.width == rect.x) {
        item.width += rect.width;
        return true;
    }
    return false;
}
