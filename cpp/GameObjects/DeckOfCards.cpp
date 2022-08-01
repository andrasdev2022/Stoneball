#include "DeckOfCards.h"
#include "Global.h"
#include "Application.h"
#include "Tileset.h"
#include "TilesetCardHandler.h"

namespace {
}

DeckOfCards::DeckOfCards(double x, double bottomY) noexcept
: BlueTriangle(x, bottomY, GameObject::Type::GOT_DECK_OF_CARDS)
{
}

BlueTriangle* DeckOfCards::copy() const noexcept {
    return new DeckOfCards(*this);
}

// SinglePlayer and clients run it
void DeckOfCards::draw(Screen* screen) noexcept {
    BlueTriangle::draw(screen);
}

bool DeckOfCards::onClicked() noexcept {
    if(Application::tileset()->tilesetCardHandler() && !Application::tileset()->tilesetCardHandler()->anyCardVisible()) {
        // The available cards should be showed up here
        //Application::tileset()->toggleCardsVisibility(id_);
        Application::tileset()->tilesetCardHandler()->toggleCardsVisibility(id_);
        return true;
    }
    return false;
}

bool DeckOfCards::onTouchEvent(int action, [[maybe_unused]]double x, double y) noexcept {
    //debug("DeckOfCards::onTouchEvent");

    if(!isTouchValid(action, x, y)) return false;

    x += screenPosX();
    y += screenPosY();


    if(action == ACTION_UP) {
        return onClicked();
    }
    return false;
}

bool DeckOfCards::isTouchValid([[maybe_unused]]int action, [[maybe_unused]]double x, double y) noexcept {
    if(Application::tileset()->tilesetCardHandler() && Application::tileset()->tilesetCardHandler()->anyCardVisible()) {
        return false;
    }

    x += screenPosX();
    y += screenPosY();

    if(x < x_ - width_ / 2 || x > x_ + width_ * 1.5 || y < y_ - height_ / 2 || y > y_ + height_ * 1.5) {
        return false;
    }
    return true;
}

DeckOfCards::Item DeckOfCards::selectedItem() const noexcept {
    if(selectedLineNr_ >= 0 && (int)items_.size() > selectedLineNr_) {
        return items_[selectedLineNr_];
    }
    return DeckOfCards::Item();
}
