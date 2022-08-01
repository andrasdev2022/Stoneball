#include "Questionmark.h"
#include "Serialize.h"
#include "Memory.h"
#include "UDPMessageData.h"
#include "SoundEffect.h"
#include "Tileset.h"
#include "Application.h"
#include "World.h"
#include "MediaData.h"

Questionmark::Questionmark(double delayMs, double timeoutMs, uint32_t linkedGameObjectId)
: BlueTriangle(0, 0, GameObject::Type::GOT_QUESTIONMARK)
, delayMs_(delayMs + (rand() % 200))
, timeoutMs_(delayMs_ + timeoutMs)
, linkedGameObjectId_(linkedGameObjectId)
{

}

BlueTriangle* Questionmark::copy() const noexcept {
    return new Questionmark(*this);
}

double Questionmark::z() const noexcept {
    return 1000. + GameObject::z();
}

void Questionmark::update(double now) noexcept {
    BlueTriangle::update(now);
    if(startTime_ < 1e-5) {
        startTime_ = now;
    }

    if(!visible()) {
        latestVisiblity_ = false;
    } else if(!latestVisiblity_) {
        latestVisiblity_ = true;
        SoundEffect::play(SoundEffect::Name::PLAY_POP);
    }

    if(startTime_ + timeoutMs_ < lastUpdateTmps) {
        die();
    }

    // Server side y calculation is done base on this calculation
    // the client side z-index will be correct with this one,
    // otherwise y_ doesn't matter, because it is recalculate on
    // client side
    auto& world = Application::latestWorld();
    if(GameObject* linkedGameObject = world->findGameObject(linkedGameObjectId_)) {
        y_ = linkedGameObject->y();
    }
}

void Questionmark::draw(Screen* screen) noexcept {
    if(!visible()) {return;}

    // This calculation is here in order to calculate a precise position on multiplayer client.
    // The position doesn't matter on server side.
    auto& world = Application::latestWorld();
    if(GameObject* linkedGameObject = world->findGameObject(linkedGameObjectId_)) {
        auto linkedGameObjectFrameData = linkedGameObject->frameData();
        auto& cRect = linkedGameObjectFrameData->contentRect();
        auto& cRect2 = frameData()->contentRect();
        double x = linkedGameObject->x() + cRect.x;
        double y = linkedGameObject->y() + cRect.y;

        x_ = x + (cRect.width - cRect2.width) / 2.;
        y_ = y - cRect2.height;
    }

    BlueTriangle::draw(screen);
}

bool Questionmark::visible() const noexcept {
    return startTime_ + delayMs_ <= lastUpdateTmps;
}

bool Questionmark::addData(Serialize* serialize) {
    return BlueTriangle::addData(serialize) &&
           serialize->add((uint8_t)visible()) != Serialize::SERIALIZE_ERROR &&
           serialize->add(linkedGameObjectId_) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* Questionmark::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = BlueTriangle::buildFrom(messageData, src);
    src = Memory::copy(messageData->visible, src);
    src = Memory::copy(messageData->linkedGameObjectId, src);
    return src;
}

void Questionmark::setVisible(bool v) noexcept {
    if(v) {
        if(!visible()) {SoundEffect::play(SoundEffect::Name::PLAY_POP);}
        delayMs_ = lastUpdateTmps - startTime_ - 1e-5;
    } else {
        delayMs_ = lastUpdateTmps - startTime_ + 1e+8;
    }
}
