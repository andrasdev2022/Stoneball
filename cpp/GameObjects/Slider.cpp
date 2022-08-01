#include "Slider.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include "MediaData.h"
#include "Application.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "Screen.h"
#include "BitmapIds.h"
#include "Bitmap565.h"
#include "Memory.h"

namespace {
const int lineSpace = 3_ZRD;
}

Slider::Slider(double x, double bottomY, int minimum, int maximum) noexcept
: BlueTriangle(x, bottomY, GameObject::Type::GOT_SLIDER)
, minimum_(minimum)
, maximum_(maximum)
{
}

BlueTriangle* Slider::copy() const noexcept {
    return new Slider(*this);
}

// SinglePlayer and clients run it
void Slider::draw(Screen* screen) noexcept {
    double y = (int)y_ - screen->y() + height_ / 2;
    uint16_t*  dstLine = screen->content() + (int)y * screen->width() + ((int)x_ - screen->x());

    const uint16_t red = Bitmap565::make565(0xde, 0x7e, 0x2a);
    Memory::memset(dstLine, red, width_);
    Memory::memset(dstLine + screen->width(), red, width_);

    double rate = (width_ - bitmapWidth(NATIVE_BITMAP_SLIDER)) / (double)(maximum_ - minimum_);
    double x = x_ + (value_ - minimum_) * rate;
    Rect rect(x, y_, 0, 0);
    rect.width  = bitmapWidth(NATIVE_BITMAP_SLIDER);
    rect.height = bitmapHeight(NATIVE_BITMAP_SLIDER);

    Bitmap565::copyIntersectionRarematrix(screen->content(), screen->rect(), screen->rect().width,
                     bitmapPtr(NATIVE_BITMAP_SLIDER, false), rect);
}


bool Slider::onTouchEvent(int action, double x, double y) noexcept {
    //x += screenPosX();
    //y += screenPosY();

    int widthB  = bitmapWidth(NATIVE_BITMAP_SLIDER);
    int heightB = bitmapHeight(NATIVE_BITMAP_SLIDER);
    double rate = (double)(maximum_ - minimum_) / (width_ - widthB);
    double rate2 = (width_ - widthB)/ (double)(maximum_ - minimum_);
    double buttonX = x_ + (value_ - minimum_) * rate2;
    //Rect rect(x, y_, 0, 0);

    if((x < buttonX || x > buttonX + widthB || y < y_ || y > y_ + heightB) && !grabbed_) {
        if(action == ACTION_DOWN && x < x_ + width_ && x > x_ && y < y_ + height_ && y > y_) {
            //debug("onTouchEvent ignored, x: %f, buttonX: %f, x_: %f, width: %d, minimum_: %d", x, buttonX, x_, width_, minimum_);
            grabbed_ = true;
            grabbedX_ = x_ + widthB / 2;

            value_ = (x - grabbedX_) * rate;
            if(value_ < minimum_) value_ = minimum_;
            if(value_ > maximum_) value_ = maximum_;
            for(auto& c : valueChangedCallbacks_) {c(this, value_);}
            return true;
        }
        //grabbed_ = false;
        return false;
    }
    //debug("onTouchEvent");
    if(action == ACTION_DOWN) {
        // list is grabbed
        //debug("ACTION_DOWN");
        grabbed_ = true;
        grabbedX_ = x - value_ / rate;
        return true;
    } else if(action == ACTION_UP) {
        //debug("ACTION_UP");
        grabbed_ = false;
        return true;
    } else if(action == ACTION_MOVE) {
        //debug("ACTION_MOVE");
        if(grabbed_) {
            value_ = (x - grabbedX_) * rate;
            if(value_ < minimum_) value_ = minimum_;
            if(value_ > maximum_) value_ = maximum_;
            for(auto& c : valueChangedCallbacks_) {c(this, value_);}
        }
    }
    return false;
}

bool Slider::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

