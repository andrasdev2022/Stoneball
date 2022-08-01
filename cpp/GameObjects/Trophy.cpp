#include "Trophy.h"

Trophy::Trophy(double x, double bottomY, GameObjectInterface::Type type) noexcept
: BlueTriangle(x, bottomY, type)
{
    init();
}

BlueTriangle* Trophy::copy() const noexcept {
    return new Trophy(*this);
}

void Trophy::update(double now) noexcept {
    int frameIndexBak = frameIndex_;
    BlueTriangle::update(now);
    if(frameIndexBak > frameIndex_) {
        frameIndex_ = frameIndexBak;
        if(time2Die_ < 1e-7) {
            time2Die_ = now;
        } else if(time2Die_ + 3e3 < now) {
            die();
        }
    }
}

const GameObjectDescriptor& Trophy::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}
