#include "Card.h"
#include "Global.h"
#include "Application.h"
#include "Tileset.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "Screen.h"
#include "TilesetCardHandler.h"
#include "UDPMessageData.h"
#include "Memory.h"

Card::Card(double x, double bottomY, GameObject::Type type, double coolDownTime) noexcept
: BlueTriangle(x, bottomY, type)
, coolDownTime_(coolDownTime)
{
}

void Card::drawAmount(Screen* screen) noexcept {
    auto& font12px = Application::font12px();
    font12px->setColor(0x0, 0x0, 0x0);
    std::wstring textAmount = std::to_wstring(amount_);

    int textWidthAmount = (*font12px).width(textAmount);
    int textHeightAmount = (*font12px).height(textAmount);

    int xposAmount = floor(x_ + width_ - textWidthAmount - 6_ZRD);
    int yposAmount = floor(y_ + height_ - textHeightAmount);

    (*font12px).draw(screen, xposAmount, yposAmount, textAmount);
    font12px->setColor(0x6f, 0xf0, 0x10);
    (*font12px).draw(screen, xposAmount - 1, yposAmount - 1, textAmount);
}

void Card::drawCoolDownTime(Screen* screen) noexcept {
    auto& font = Application::font();
    font->setColor(0xff, 0xe0, 0x00);
    std::wstring text = std::to_wstring((int)(coolDown_ / 1e3));

    int textWidth = (*font).width(text);
    int textHeight = (*font).height(text);

    int xpos = floor(x_ + (width_ - textWidth) / 2);
    int ypos = floor(y_ + (height_ - textHeight) / 2);

    (*font).draw(screen, xpos, ypos, text);
}

// SinglePlayer and clients run it
void Card::draw(Screen* screen) noexcept {
    if(!visible_ && hideTimer_ + 400 < Application::now()) return;
    BlueTriangle::draw(screen);
    drawAmount(screen);
    if(coolDown_ <= 0. || Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) return;
    drawCoolDownTime(screen);
}

void Card::update(double now) noexcept {
    if(lastUpdateTmps > 1e-3) {
        coolDown_ -= now - lastUpdateTmps;
    }
    if(coolDown_ <= 0.) {
        coolDown_ = 0.;
        setBitmapNormal();
    } else if(Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
        setBitmapDisabled();
    }
    BlueTriangle::update(now);
}

bool Card::onClicked() noexcept {
    debug("Card::onClicked");
    if(coolDown_ > 500 && Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
        double egt = Application::tileset()->elapsedGameTime(teamIndex_);
        //debug("Card::onClicked: elapsed game time: %f, coolDown: %f", egt, coolDown_);
        if(ceil(coolDown_ / 1e3) <= Application::playTime() * 60. - egt) {
            Application::tileset()->setElapsedGameTime(teamIndex_, (uint16_t)(egt + ceil(coolDown_ / 1e3)));
            coolDown_ = 0.;
        }
    }
    if(coolDown_ > 500 && Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) return true;
    debug("card id: %u has been clicked", id_);
    setActive(!active_);
    Application::tileset()->tilesetCardHandler()->hideAllCards();
    return true;
}

bool Card::onTouchEvent(int action, [[maybe_unused]]double x, double y) noexcept {
    if(!visible_) return false;

    if(!isTouchValid(action, x, y)) return false;

    x += screenPosX();
    y += screenPosY();

    if(action == ACTION_UP) {
        onClicked();
        return true;
    }
    return false;
}

bool Card::isTouchValid([[maybe_unused]]int action, [[maybe_unused]]double x, double y) noexcept {
    //debug("DeckOfCards::onTouchEvent");
    x += screenPosX();
    y += screenPosY();

    if(x < x_ || x > x_ + width_ || y < y_ || y > y_ + height_) {
        return false;
    }
    return true;
}

bool Card::addData(Serialize* serialize) {
    return BlueTriangle::addData(serialize) &&
           serialize->add((uint8_t)visible_) != Serialize::SERIALIZE_ERROR &&
           serialize->add(teamIndex_) != Serialize::SERIALIZE_ERROR &&
           serialize->add(amount_) != Serialize::SERIALIZE_ERROR &&
           serialize->add(coolDown_) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* Card::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = BlueTriangle::buildFrom(messageData, src);
    src = Memory::copy(messageData->visible, src);
    src = Memory::copy(messageData->teamIndex, src);
    src = Memory::copy(messageData->amount, src);
    src = Memory::copy(messageData->coolDown, src);
    return src;
}

void Card::hide() noexcept {
    if(visible_) {
        hideTimer_ = Application::now();
    }
    visible_ = false;
}

void Card::setVisible(bool v) noexcept {
    if(visible_ && !v) {
        hideTimer_ = Application::now();
    }
    visible_ = v;
}
