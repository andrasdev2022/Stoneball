#include "Selectionbox.h"
#include "Application.h"
#include <codecvt>
#include <locale>
#include "ObserverTouchEventSubject.h"
#include "MediaData.h"
#include "Global.h"

constexpr int NO_BUTTON = -1;
constexpr int LEFT_BUTTON = 0;
constexpr int RIGHT_BUTTON = 1;

Selectionbox::Selectionbox(double x, double bottomY) noexcept
: BlueTriangle(x, bottomY, GameObject::Type::GOT_SELECTIONBOX)
{

}

BlueTriangle* Selectionbox::copy() const noexcept {
    return new Selectionbox(*this);
}

// GameObject
void Selectionbox::draw(Screen* screen) noexcept {
    for(auto& c : drawBeginCallbacks_) {c(this);}

    auto& f = Application::font();
    (*f).setColor(0xdd, 0xc6, 0x9f);

    if(selectedLineNr_ >= 0 && selectedLineNr_ < (int)items_.size()) {
        int width = (*f).width(items_[selectedLineNr_].label);
        int height = (*f).height(items_[selectedLineNr_].label);
        int xOffset = (width_ - width) / 2;
        int yOffset = (height_ - height) / 2;
        //debug("Selectionbox::draw: height: %d, height_: %d", height, height_);
        //std::wcout << items_[selectedLineNr_].label << std::endl;
        (*f).draw(screen, x_ + xOffset, y_ + yOffset + 3_ZRD, items_[selectedLineNr_].label);
    }

    for(auto& c : drawEndCallbacks_) {c(this);}
}

bool Selectionbox::onTouchEvent(int action, double x, double y) noexcept {
    //x += screenPosX();
    //y += screenPosY();
    int buttonNr = NO_BUTTON;
    //if(x >= 228._ZRW && x <= 239._ZRW && y >= 138._ZRH && y <= 160._ZRH) {
    if(x >= x_ - 25._ZRD && x <= x_ + 15_ZRD && y >= y_ - height_ * 0.8 && y <= y_ + height_ * 1.3) {
        // left button
        buttonNr = LEFT_BUTTON;
        //debug("Selectionbox::onTouchEvent buttonNr = LEFT_BUTTON");
    //} else if(x >= 434._ZRW && x <= 446._ZRW && y >= 138._ZRH && y <= 160._ZRH) {
    } else if(x >= x_ + width_ - 15_ZRD && x <= x_ + width_ + 20._ZRD && y >= y_ - height_ * 0.8 && y <= y_ + height_ * 1.3) {
        // right button
        buttonNr = RIGHT_BUTTON;
        //debug("Selectionbox::onTouchEvent buttonNr = RIGHT_BUTTON");
    }

    if(action == ACTION_DOWN) {
        //debug("Selectionbox::onTouchEvent x_: %f, y_: %f, x: %f, y: %f, width_: %d. height_: %d", x_, y_, x, y, width_, height_);
        if(buttonNr != NO_BUTTON) {
            pushedButton_ = buttonNr;
            //debug("Selectionbox::onTouchEvent button pushed: %d", pushedButton_);
        }
    } else if(action == ACTION_UP) {
        if(pushedButton_ != NO_BUTTON && buttonNr == pushedButton_) {
            //debug("Selectionbox::onTouchEvent button released: %d", pushedButton_);
            if(pushedButton_  == LEFT_BUTTON && selectedLineNr_ > 0) {
                --selectedLineNr_;
            } else if(pushedButton_  == RIGHT_BUTTON && selectedLineNr_ < (int)items_.size() - 1) {
                ++selectedLineNr_;
            }
            for(auto& c : selectionCallbacks_) {
                c(this, selectedLineNr_);
            }
        }
        pushedButton_ = NO_BUTTON;
    } else if(action == ACTION_MOVE) {
    }
    return false;
}

bool Selectionbox::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

Selectionbox::Item Selectionbox::selectedItem() const noexcept {
    if(selectedLineNr_ < (int)items_.size()) {
        return items_.at(selectedLineNr_);
    }
    return Item();
}

void Selectionbox::initOffsets(int xOffset, int yOffset) noexcept {
    if(!selectionboxOffsetUpdated_) {
        //debug("Selectionbox::initOffsets DBG 1 x_: %f, x(): %f, xOffset: %d", x_, x(), xOffset);
        setX(x() + xOffset);
        //debug("Selectionbox::initOffsets DBG 2 x: %f", x());
        setY(y() + yOffset);
        //debug("Selectionbox::initOffsets xOffset: %d, yOffset: %d, x: %f", xOffset, yOffset, x());
        selectionboxOffsetUpdated_ = true;
    }
}
