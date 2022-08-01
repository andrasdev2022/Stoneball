#include "MessageboxTermsOfUse.h"
#include "Screen.h"
#include "BitmapIds.h"
#include "Bitmap565.h"
#include "Application.h"
#include "MediaData.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "TouchEventHandler.h"

MessageboxTermsOfUse::MessageboxTermsOfUse()
: BlueTriangle(0, 0, GameObject::Type::GOT_MESSAGEBOXTERMSOFUSE) {
}

BlueTriangle* MessageboxTermsOfUse::copy() const noexcept {
    return new MessageboxTermsOfUse(*this);
}

double MessageboxTermsOfUse::z() const noexcept {
    return 2500. + GameObject::z();
}

// GameObject
void MessageboxTermsOfUse::update(double now) noexcept {
    BlueTriangle::update(now);
}

void MessageboxTermsOfUse::drawBackground(Screen* screen, int bitmapId, int x, int y) noexcept {
    const MediaData* bData = bitmapData(bitmapId, false);
    Bitmap565::copyIntersection(screen->content(),
                                screen->rect(),
                                screen->rect().width,
                                bData->data(),
                                Rect(x, y, bData->width(), bData->height()),
                                bData->width(),
                                true,
                                100);
}

void MessageboxTermsOfUse::draw(Screen* screen) noexcept {
    x_ = screenPosX() + (screen->width() - width_) / 2.;
    y_ = screenPosY() + (screen->height() - height_) / 2;
    drawBackground(screen, frameIndex_, x_, y_);
}

bool MessageboxTermsOfUse::onTouchEvent(int action, double x, double y) noexcept {
    x -= (screenWidth() - width_) / 2;
    y -= (screenHeight() - height_) / 2;

    Button button = Button::UNDEFINED;

    if(x >= 105_ZRD && y >= 91_ZRD && x <= 195_ZRD && y <= 108_ZRD) button = Button::ACCEPT;
    if(x >= 135_ZRD && y >= 74_ZRD && x <= 167_ZRD && y <= 86_ZRD) button = Button::READ;

    if(button == Button::UNDEFINED) return true;

    if(action == ACTION_DOWN) {
        pushed_ = true;
    } else if(action == ACTION_UP) {
        pushed_ = false;
        //Application::touchEventHandler()->remove(this);
        //die();
        for(auto& v : pushButtonCallbacks_) {
            v(this, button);
        }
    }
    // this is a modal dialog, nothing else should receive touch events
    return true;
}

bool MessageboxTermsOfUse::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

