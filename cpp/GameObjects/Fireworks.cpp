#include "Fireworks.h"

Fireworks::Fireworks(double x, double bottomY, GameObject::Type type) noexcept
: BlueTriangle(x, bottomY, type)
{
    init();
}

BlueTriangle* Fireworks::copy() const noexcept {
    return new Fireworks(*this);
}

void Fireworks::update(double now) noexcept {
    int frameIndexBak = frameIndex_;
    BlueTriangle::update(now);
    if(frameIndexBak > frameIndex_) {die();}
}

const GameObjectDescriptor& Fireworks::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}
