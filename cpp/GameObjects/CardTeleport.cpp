#include "CardTeleport.h"
#include "BitmapIds.h"
#include "Application.h"

CardTeleport::CardTeleport(double coolDownTime) noexcept
: Card(0, 0, GameObject::Type::GOT_CARD_TELEPORT, coolDownTime)
{
}

BlueTriangle* CardTeleport::copy() const noexcept {
    return new CardTeleport(*this);
}

void CardTeleport::setActive(bool active) noexcept {
    // it's not deactivable in single player mode
    //if(Card::active() && Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) return;
    Card::setActive(active);
    if(active) {
        skinMin_ = NATIVE_BITMAP_CARD_TELEPORT_ACTIVE;
    } else {
        skinMin_ = NATIVE_BITMAP_CARD_TELEPORT;
    }
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardTeleport::setBitmapDisabled() noexcept {
    skinMin_ = NATIVE_BITMAP_CARD_TELEPORT_DISABLED;
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardTeleport::setBitmapNormal() noexcept {
    if(skinMin_ == NATIVE_BITMAP_CARD_TELEPORT_DISABLED) {
        skinMin_ = NATIVE_BITMAP_CARD_TELEPORT;
        skinMax_ = skinMin_;
        frameIndex_ = skinMin_;
    }
}
