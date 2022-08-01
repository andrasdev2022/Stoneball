#include "EmoteFigure.h"
#include "Serialize.h"
#include "Memory.h"
#include "UDPMessageData.h"
#include "SoundEffect.h"
#include "BitmapIds.h"

EmoteFigure::EmoteFigure(double delayMs, double timeoutMs, uint32_t emoteId, uint8_t avatar)
: BlueTriangle(0, 0, GameObject::Type::GOT_EMOTE_FIGURE)
, delayMs_(delayMs + (rand() % 200))
, timeoutMs_(delayMs_ + timeoutMs)
, emoteId_(emoteId)
, avatar_(avatar)
{
    changeDescriptor();
    init();
}

BlueTriangle* EmoteFigure::copy() const noexcept {
    return new EmoteFigure(*this);
}

double EmoteFigure::z() const noexcept {
    return 1000. + GameObject::z();
}

void EmoteFigure::update(double now) noexcept {
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
}

void EmoteFigure::draw(Screen* screen) noexcept {
    if(!visible()) {return;}
    BlueTriangle::draw(screen);
}

bool EmoteFigure::visible() const noexcept {
    return startTime_ + delayMs_ <= lastUpdateTmps;
}

bool EmoteFigure::addData(Serialize* serialize) {
    return BlueTriangle::addData(serialize) &&
           serialize->add((uint8_t)visible()) != Serialize::SERIALIZE_ERROR &&
           serialize->add(avatar_) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* EmoteFigure::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = BlueTriangle::buildFrom(messageData, src);
    src = Memory::copy(messageData->visible, src);
    src = Memory::copy(messageData->teamIndex, src); // avatar
    return src;
}

void EmoteFigure::setVisible(bool v) noexcept {
    if(v) {
        if(!visible()) {SoundEffect::play(SoundEffect::Name::PLAY_POP);}
        delayMs_ = lastUpdateTmps - startTime_ - 1e-5;
    } else {
        delayMs_ = lastUpdateTmps - startTime_ + 1e+8;
    }
}

const GameObjectDescriptor& EmoteFigure::god() const noexcept {
    return descriptor_;
}

void EmoteFigure::changeDescriptor() noexcept {
    descriptor_ = GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
    uint32_t newSkin = NATIVE_BITMAP_EMOTE_200IQ;
    if(emoteId_ < 6) newSkin += emoteId_;
    descriptor_.setSkinStart(newSkin);
    descriptor_.setSkinMin(newSkin);
    descriptor_.setSkinMax(newSkin);
}

void EmoteFigure::setEmoteId(uint32_t emoteId) {
    emoteId_ = emoteId;
    changeDescriptor();
}
