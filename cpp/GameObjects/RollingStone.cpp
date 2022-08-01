#include "RollingStone.h"
#include "stoneball.h"
#include <math.h>
#include "MediaData.h"
#include "Collision.h"
#include <iostream>
#include "Vector.h"
#include "Application.h"
#include "Tileset.h"
#include "Level1.h"
#include "BitmapIds.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"

RollingStone::RollingStone(double x, double bottomY, GameObjectInterface::Type type, Color color) noexcept
: GameObject(x, bottomY, type)
, color_(color)
{
    changeDescriptor();
    init();
}

void RollingStone::interateSkinMin() noexcept {
    switch(color_) {
    case Color::Gray:
        skinMin_ = (skinMin_ == NATIVE_BITMAP_STONEBALL_01 ? NATIVE_BITMAP_STONEBALL_02 : NATIVE_BITMAP_STONEBALL_01);
        break;
    case Color::Red:
        skinMin_ = (skinMin_ == NATIVE_BITMAP_STONEBALL_RED_01 ? NATIVE_BITMAP_STONEBALL_RED_02 : NATIVE_BITMAP_STONEBALL_RED_01);
        break;
    case Color::Blue:
        skinMin_ = (skinMin_ == NATIVE_BITMAP_STONEBALL_BLUE_01 ? NATIVE_BITMAP_STONEBALL_BLUE_02 : NATIVE_BITMAP_STONEBALL_BLUE_01);
        break;
    }
}

void RollingStone::resetSkinMin() noexcept {
    switch(color_) {
    case Color::Gray:
        skinMin_ = NATIVE_BITMAP_STONEBALL_01;
        break;
    case Color::Red:
        skinMin_ = NATIVE_BITMAP_STONEBALL_RED_01;
        break;
    case Color::Blue:
        skinMin_ = NATIVE_BITMAP_STONEBALL_BLUE_01;
        break;
    }
}

void RollingStone::update(double now) noexcept {
    double oldX = x_;
    double oldY = y_;
    if(isRolling()) {
        /*
        if(skinMin_ == NATIVE_BITMAP_STONEBALL_01) {
            skinMin_ = NATIVE_BITMAP_STONEBALL_02;
        } else {
            skinMin_ = NATIVE_BITMAP_STONEBALL_01;
        }
        */
        interateSkinMin();
        skinMax_ = skinMin_;
        frameIndex_ = skinMin_;
    }
    updateFrame(now, skinMin_, skinMax_);
    walkOrMove(now);

    if(isRolling()) {
        bool stop = false;
        if(direction_.x > .5 && (x_ > oldX && x_ > stopAt_.x)) {
            stop = true;
        } else if(direction_.x < -.5 && (x_ < oldX && x_ < stopAt_.x)) {
            stop = true;
        } else if(direction_.y > .5 && (y_ > oldY && y_ > stopAt_.y)) {
            stop = true;
        } else if(direction_.y < -.5 && (y_ < oldY && y_ < stopAt_.y)) {
            stop = true;
        }
        if(stop) {
            direction_ = VectorD();
            x_ = stopAt_.x;
            y_ = stopAt_.y;
            velocity_ = 0.;
            //skinMin_ = NATIVE_BITMAP_STONEBALL_01;
            resetSkinMin();
            skinMax_ = skinMin_;
            frameIndex_ = skinMin_;
            stopAt_ = VectorD();
            Application::tileset()->incStonePushCounter();

            // If this stone is close enough to any hole, the observers (Environment) must be notified
            for(auto r : holeRects_) {
                Vector v(x_ + width() / 2., y_ + height(), r.x + r.width / 2., r.y + r.height / 2.);
                if(v.length() < 10_ZRD) {
                    //debug("RollingStone::update x: %f, y: %f, x: %d, y: %d, width: %d, height: %d\n", x_, y_, r.x, r.y, r.width, r.height);
                    collided_ = r;
                    notify();
                    break;
                }
            }
        }
    }

}

const GameObjectDescriptor& RollingStone::god() const noexcept {
    return descriptor_;
}

void RollingStone::push(double x, double y, double velocity) noexcept {
    // Faster move than the hero's and the pushed sitting knight's speed
    velocity_ = velocity * 2.19;

    // one length vector
    double length = VectorD(x,y).length();
    direction_.x = x / length;
    direction_.y = y / length;

    // No more collision before the new position
    x_ += direction_.x;
    y_ += direction_.y;

    // the stone's position must be followed on the tileset
    auto* tileset = Application::tileset();
    double specX = x_;
    double specY = y_;
    if(direction_.x > .5) {
        specX += tileset->tileWidth();
    }
    if(direction_.y > .5) {
        specY += tileset->tileHeight();
    }
    tileset->updateStonePosition(id_, specX, specY, color_);
    stopAt_ = VectorD(tileset->convertTileX2StoneX(tileset->convertStoneX2TileX(specX)), tileset->convertTileY2StoneY(tileset->convertStoneY2TileY(specY)));
}

void RollingStone::changeDescriptor() noexcept {
    descriptor_ = GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
    switch(color_) {
    case Color::Gray:
        descriptor_.setSkinStart(NATIVE_BITMAP_STONEBALL_01);
        descriptor_.setSkinMin(NATIVE_BITMAP_STONEBALL_01);
        descriptor_.setSkinMax(NATIVE_BITMAP_STONEBALL_01);
        break;
    case Color::Red:
        descriptor_.setSkinStart(NATIVE_BITMAP_STONEBALL_RED_01);
        descriptor_.setSkinMin(NATIVE_BITMAP_STONEBALL_RED_01);
        descriptor_.setSkinMax(NATIVE_BITMAP_STONEBALL_RED_01);
        break;
    case Color::Blue:
        descriptor_.setSkinStart(NATIVE_BITMAP_STONEBALL_BLUE_01);
        descriptor_.setSkinMin(NATIVE_BITMAP_STONEBALL_BLUE_01);
        descriptor_.setSkinMax(NATIVE_BITMAP_STONEBALL_BLUE_01);
        break;
    }
}

bool RollingStone::addData(Serialize* serialize) {
    return GameObject::addData(serialize) &&
           serialize->add((uint8_t)'S') != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)State::Type::ST_IDLE) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)color()) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* RollingStone::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    //src = GameObject::buildFrom(messageData, src);
    //src = Memory::copy(messageData->type, src);
    src = Memory::copy(messageData->state, src);
    src = Memory::copy(messageData->color, src);
    return src;
}
