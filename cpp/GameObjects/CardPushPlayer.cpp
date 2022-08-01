#include "CardPushPlayer.h"
#include "BitmapIds.h"
#include "Application.h"

CardPushPlayer::CardPushPlayer(double coolDownTime) noexcept
: Card(0, 0, GameObject::Type::GOT_CARD_PUSH_PLAYER, coolDownTime)
{
}

BlueTriangle* CardPushPlayer::copy() const noexcept {
    return new CardPushPlayer(*this);
}

void CardPushPlayer::setActive(bool active) noexcept {
    // it's not deactivable in single player mode
    //if(Card::active() && Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) return;
    Card::setActive(active);
    if(active) {
        skinMin_ = NATIVE_BITMAP_CARD_PUSH_PLAYER_ACTIVE;
    } else {
        skinMin_ = NATIVE_BITMAP_CARD_PUSH_PLAYER;
    }
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardPushPlayer::setBitmapDisabled() noexcept {
    skinMin_ = NATIVE_BITMAP_CARD_PUSH_PLAYER_DISABLED;
    skinMax_ = skinMin_;
    frameIndex_ = skinMin_;
}

void CardPushPlayer::setBitmapNormal() noexcept {
    if(skinMin_ == NATIVE_BITMAP_CARD_PUSH_PLAYER_DISABLED) {
        skinMin_ = NATIVE_BITMAP_CARD_PUSH_PLAYER;
        skinMax_ = skinMin_;
        frameIndex_ = skinMin_;
    }
}
