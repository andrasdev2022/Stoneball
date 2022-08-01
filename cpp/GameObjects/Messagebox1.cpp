#include "Messagebox1.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include <iostream>
#include "Algorithms.h"
#include "Application.h"
#include "TranslateInterface.h"
#include "UDPMessageData.h"
#include "Memory.h"
#include "Serialize.h"
#include "StringUtil.h"
#include "BitmapIds.h"
#include "Global.h"
#include "MediaData.h"

using namespace  StringUtil;

Messagebox1::Messagebox1(double timeoutMs, TranslateInterface::Key messageKey) noexcept
: BlueTriangle(0, 0, GameObject::Type::GOT_MESSAGEBOX)
, timeoutMs_(timeoutMs)
, message_(messageKey)
{
}

Messagebox1::Messagebox1(double timeoutMs, TranslateInterface::Key messageKey, std::wstring message) noexcept
: BlueTriangle(0, 0, GameObject::Type::GOT_MESSAGEBOX)
, timeoutMs_(timeoutMs)
, message_(messageKey)
, message2_(message)
{
}

BlueTriangle* Messagebox1::copy() const noexcept {
    return new Messagebox1(*this);
}

double Messagebox1::z() const noexcept {
    return 2300. + GameObject::z();
}

void Messagebox1::update(double now) noexcept {
    BlueTriangle::update(now);
    if(startTime_ < 1e-5) {
        startTime_ = lastUpdateTmps;
    }

    if(startTime_ + timeoutMs_ < lastUpdateTmps) {
        die();
    } else {
        spareTime_ = startTime_ + timeoutMs_ - now;
    }
}

// SinglePlayer and multiplayer clients run it
void Messagebox1::draw(Screen* screen) noexcept {

    // Messagebox is fixed to the middle of the screen, the original position doesn't matter
    int messageboxX = (screenWidth() - bitmapWidth(NATIVE_BITMAP_MESSAGEBOX1)) / 2;
    int messageboxY = (screenHeight() - bitmapHeight(NATIVE_BITMAP_MESSAGEBOX1)) / 2;

    if(Algorithms::anyOf(Application::activeMenu(), MenuInterface::MenuType::SINGLEPLAYER_RUNNIG,
                         MenuInterface::MenuType::MULTIPLAYER_HOST_RUNNING,
                         MenuInterface::MenuType::MULTIPLAYER_JOINED_RUNNING,
                         MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING)) {
        messageboxX += screenPosX();
        messageboxY += screenPosY();
    }
    setX(messageboxX);
    setY(messageboxY);

    BlueTriangle::draw(screen);

    auto f = &Application::font();
    auto& tr = Application::translator();
    std::wstring message;
    if(message_ != TranslateInterface::Key::LAST_ONE) {
        message = (*tr)(message_);
        wstr_replace(message, L"%s", message2_);
        if(Application::language() == TranslateInterface::Language::ENGLISH) {
            wstr_replace(message, L"s's", L"s'");
        }
    } else {
        message = message2_;
    }

    if(message.length() > 30) {
        f = &Application::font14px();
    }

    int textWidth = (**f).width(message);
    int textHeight = (**f).height(message);

    int textXPos = x_ + (width() - textWidth) / 2;
    int textYPos = y_ + (height() - textHeight) / 2;

    (**f).setColor(0x77, 0x24, 0x00);
    (**f).draw(screen, textXPos, textYPos, message);
    if(showCountDown_) {
        drawCountDown(screen);
    }
}

void Messagebox1::drawCountDown(Screen* screen) noexcept {
    auto& f = Application::font8px();

    int minutes = floor(spareTime_ / 6e4);
    int seconds = floor(spareTime_ / 1e3) - minutes * 60;

    std::wstring min = std::to_wstring(minutes);
    std::wstring sec = std::to_wstring(seconds);
    if(min.length() == 1) {
        min = L"0" + min;
    }
    if(sec.length() == 1) {
        sec = L"0" + sec;
    }
    std::wstring timeStr = min + L":" + sec;
    int textWidth = (*f).width(timeStr);
    //int textHeight = (*f).height(timeStr);

    int textXPos = x_ + (width() - textWidth) / 2;
    int textYPos = y_ + height() - 10_ZRD;

    (*f).setColor(0xdf, 0xdf, 0xdf);
    (*f).draw(screen, textXPos, textYPos, timeStr);
}

bool Messagebox1::addData(Serialize* serialize) {
    return BlueTriangle::addData(serialize) &&
           serialize->add((uint8_t)message()) != Serialize::SERIALIZE_ERROR &&
           serialize->add((uint8_t)showCountDown_) != Serialize::SERIALIZE_ERROR &&
           serialize->add(spareTime_) != Serialize::SERIALIZE_ERROR &&
           serialize->addString(message2_) != Serialize::SERIALIZE_ERROR;
}

// copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
// and MultiplayerWorld
const uint8_t* Messagebox1::buildFrom(UDPMessageData* messageData, const uint8_t* src) {
    src = BlueTriangle::buildFrom(messageData, src);
    src = Memory::copy(messageData->messageKey, src);
    src = Memory::copy(messageData->showCountDown, src);
    src = Memory::copy(messageData->spareTime, src);
    src = Memory::copy(messageData->messageString, src);
    return src;
}
