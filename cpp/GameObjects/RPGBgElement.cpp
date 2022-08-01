#include "RPGBgElement.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include <iostream>
#include "Algorithms.h"
#include "BitmapIds.h"
#include "Global.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"

RPGBgElement::RPGBgElement(double x, double y, int bitmapId) noexcept
    : GameObject(x, y + bitmapHeight(bitmapId), GameObject::Type::GOT_RPGBGELEMENT)
{
    this->direction_ = Vector<double>(0, 0.);
    // to fill the basic stuff like fly state
    changeBitmapId(bitmapId);
    descriptor_.setYPosCorrection(0.);
    descriptor_.setVelocity(0.);
    init();
}

const GameObjectDescriptor& RPGBgElement::god() const noexcept {
    //return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
    return descriptor_;
}

void RPGBgElement::update(double now) {
    updateFrame(now, skinMin_, skinMax_);
    walkOrMove(now);
}

double RPGBgElement::z() const noexcept {
    if(Algorithms::anyOf(skinMin_, NATIVE_BITMAP_LAKE_01, NATIVE_BITMAP_HOLE_01, NATIVE_BITMAP_HOLE_RED_01, NATIVE_BITMAP_HOLE_BLUE_01, NATIVE_BITMAP_HOLE_DARK_01)) return -1.;
    return GameObject::z();
}

bool RPGBgElement::addData(Serialize* serialize) {
    return GameObject::addData(serialize) &&
           serialize->add((uint8_t)'R') != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)State::Type::ST_IDLE) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* RPGBgElement::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    //src = GameObject::buildFrom(messageData, src);
    //src = Memory::copy(messageData->type, src);
    src = Memory::copy(messageData->state, src);
    return src;
}

void RPGBgElement::changeBitmapId(int bitmapId) noexcept {
    descriptor_.setSkinStart(bitmapId);
    descriptor_.setSkinMin(bitmapId);
    setFrameIndex(bitmapId);
    setSkinMin(bitmapId);
    if(Algorithms::anyOf(bitmapId, NATIVE_BITMAP_HOLE_01, NATIVE_BITMAP_HOLE_RED_01, NATIVE_BITMAP_HOLE_BLUE_01, NATIVE_BITMAP_HOLE_DARK_01)) {
        descriptor_.setSkinMax(bitmapId + 3);
        descriptor_.setAnimTimeStep(100);
        setSkinMax(bitmapId + 3);
    } else {
        descriptor_.setSkinMax(bitmapId);
        descriptor_.setAnimTimeStep(1e8);
        setSkinMax(bitmapId);
    }
}
