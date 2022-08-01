#ifndef __COLLISIONDATA_H__
#define __COLLISIONDATA_H__

#include <memory>

#include "Rect.h"

class CollisionDataVisitor;

class CollisionData
{
public:
    CollisionData(Rect rect) :rect_(rect) {}
    virtual ~CollisionData() = default;
    virtual Rect rect() const {return rect_;}
    virtual void accept(CollisionDataVisitor*) const noexcept{}

private:
    Rect rect_;
};

using CollisionDataUptr = std::unique_ptr<CollisionData>;

#endif // __COLLISIONDATA_H__
