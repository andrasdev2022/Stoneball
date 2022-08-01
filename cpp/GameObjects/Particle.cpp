#include "Particle.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include "Global.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"

Particle::Particle(double x, double bottomY, GameObject::Type type) noexcept
: GameObject(x, bottomY, type)
{
    this->direction_ = Vector<double>((ZRD(rand() % 30) - 15_ZRD ), 1.);
    // to fill the basic stuff like fly state
    init();
}

void Particle::update(double now) noexcept {
    if(bornAt < 0) bornAt = now;
    updateFrame(now, skinMin_, skinMax_);

    if(lastUpdateTmps < 1e-4) {
        lastUpdateTmps = now;
    }

    double distance = 1080._ZRD / 15000. * (now - lastUpdateTmps);
    y_ += distance * direction_.y;
    this->direction_.x -= this->direction_.x / 25.;
    this->direction_.x *= (rand() % 2) ? 1. : -1.;
    walkOrMove(now);
}

const GameObjectDescriptor& Particle::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}

void Particle::die() noexcept {
    bornAt = 1.;
}

bool Particle::isDead() const noexcept {
    if(bornAt > 0. && lastUpdateTmps - bornAt > 2000) {
        return true;
    }
    return false;
}

bool Particle::addData(Serialize* serialize) {
    return GameObject::addData(serialize) &&
           serialize->add((uint8_t)'P') != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)State::Type::ST_IDLE) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* Particle::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    //src = GameObject::buildFrom(messageData, src);
    //src = Memory::copy(messageData->type, src);
    src = Memory::copy(messageData->state, src);
    return src;
}
