#include "BlueTriangle.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include <iostream>
#include "Algorithms.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"

BlueTriangle::BlueTriangle(double x, double bottomY, GameObject::Type type) noexcept
: GameObject(x, bottomY, type)
{
    init();
}

const GameObjectDescriptor& BlueTriangle::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}

void BlueTriangle::update(double now) noexcept {
    updateFrame(now, skinMin_, skinMax_);
    walkOrMove(now);
}

double BlueTriangle::z() const noexcept {
    return 1000.;
}

BlueTriangle* BlueTriangle::copy() const noexcept {
    return new BlueTriangle(*this);
}

bool BlueTriangle::addData(Serialize* serialize) {
    return GameObject::addData(serialize) &&
           serialize->add((uint8_t)'O') != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)State::Type::ST_IDLE) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* BlueTriangle::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    //src = GameObject::buildFrom(messageData, src);
    //src = Memory::copy(messageData->type, src);
    src = Memory::copy(messageData->state, src);
    return src;
}
