#include "CardPull.h"
#include "BitmapIds.h"
#include "Application.h"

CardPull::CardPull(double coolDownTime) noexcept
: Card(0, 0, GameObject::Type::GOT_CARD_PULL, coolDownTime)
{
}

BlueTriangle* CardPull::copy() const noexcept {
    return new CardPull(*this);
}

void CardPull::setActive(bool active) noexcept {
    // it's not deactivable in single player mode
    //if(Card::active() && Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) return;
    Card::setActive(active);
    if(active) {
        skinMin_ = NATIVE_BITMAP_CARD_PULL_ACTIVE;
    } else {
        skinMin_ = NATIVE_BITMAP_CARD_PULL;
    }
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardPull::setBitmapDisabled() noexcept {
    skinMin_ = NATIVE_BITMAP_CARD_PULL_DISABLED;
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardPull::setBitmapNormal() noexcept {
    if(skinMin_ == NATIVE_BITMAP_CARD_PULL_DISABLED) {
        skinMin_ = NATIVE_BITMAP_CARD_PULL;
        skinMax_ = skinMin_;
        frameIndex_ = skinMin_;
    }
}
