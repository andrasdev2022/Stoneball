#include "Listbox1.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include <iostream>
#include "Algorithms.h"
#include "Application.h"
#include "TranslateInterface.h"
#include <codecvt>
#include <locale>
#include "Global.h"
#include <algorithm>
#include "ObserverTouchEventSubject.h"
#include "Screen.h"

namespace {
const int lineSpace = 3_ZRD;
}

Listbox1::Listbox1(double x, double bottomY, int maxHeight) noexcept
: BlueTriangle(x, bottomY, GameObject::Type::GOT_LISTBOX)
, maxHeight_(maxHeight)
{
}

BlueTriangle* Listbox1::copy() const noexcept {
    return new Listbox1(*this);
}

void Listbox1::calcSizes() noexcept {
    if(totalHeight_) return;

    auto& f = Application::font12px();
    int textHeight = 0;
    int textWidth = 0;
    int lineIndex = 0;
    for(auto& i : items_) {
        //auto wStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(i.label);
        auto wStr = i.label;
        lineHeight_ = std::max((*f).height(wStr) + lineSpace, lineHeight_);
        if(textHeight + lineHeight_ <= maxHeight_) {
            textHeight += lineHeight_;
            ++visibleLineCount_;
        }
        textWidth = std::max((*f).width(wStr), textWidth);
        totalHeight_ += lineHeight_;
        ++lineIndex;
    }
    width_ = textWidth;
    height_ = textHeight;
}

// SinglePlayer and clients run it
void Listbox1::draw(Screen* screen) noexcept {
    //BlueTriangle::draw(screen);

    auto& f = Application::font12px();
    (*f).setColor(0xe3, 0xab, 0x77);

    calcSizes();

    Screen screen2(0, 0, screen->width(), screen->height());
    screen2.setContentPtr(screen->content());
    int textHeight = 0;
    int lineIndex = 0;
    for(auto& i : items_) {
        //auto wStr = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(i.label);
        auto wStr = i.label;
        if(lineIndex == selectedLineNr_) {
            (*f).setColor(0xff, 0xd1, 0x9f);
        }
        if(textHeight + scrollY_ >= 0 && textHeight + scrollY_ <= height_) {
            (*f).draw(&screen2, x_, y_ + textHeight + scrollY_, wStr);
        }
        if(lineIndex == selectedLineNr_) {
            (*f).setColor(0xe3, 0xab, 0x77);
        }
        textHeight += lineHeight_;
        ++lineIndex;
    }
}

void Listbox1::toggleSelection([[maybe_unused]]double x, double y) noexcept {
    // scrollY_
    // maxHeight_
    // width_
    // visibleLineCount_
    if(x < x_ || x > x_ + width_ || y < y_ || y > y_ + height_) return;
    int line = (y - y_ - scrollY_) / lineHeight_;
    //debug("Selected line nr.: %d", line);
    if(line == selectedLineNr_ || line >= (int)items_.size()) {
        selectedLineNr_ = -1;
    } else {
        selectedLineNr_ = line;
    }

    for(auto& c : selectionCallbacks_) {
        c(this, selectedLineNr_);
    }
}

bool Listbox1::onTouchEvent(int action, [[maybe_unused]]double x, double y) noexcept {

    if(x < x_ || x > x_ + width_ || y < y_ || y > y_ + height_) {
        pushed_ = false;
        scrolled_ = false;
        //debug("onTouchEvent ignored");
        return false;
    }
    //debug("onTouchEvent");

    if(action == ACTION_DOWN) {
        // list is grabbed
        pushed_ = true;
        scrolled_ = false;
        grabY_ = y - scrollY_;
        //debug("ACTION_DOWN");
        return true;
    } else if(action == ACTION_UP) {
        //debug("ACTION_UP");
        // if it moved then it was scrolled
        // otherwise an item was selected
        if(pushed_) {
            //debug("dbg 1");
            if(scrolled_) {
                //debug("dbg 2");
                // nothing to do
            } else {
                //debug("dbg 3");
                // selection or deselection
                toggleSelection(x, y);
            }
        }
        pushed_ = false;
        return true;
    } else if(action == ACTION_MOVE) {
        //debug("ACTION_MOVE");
        // scroll
        //debug("y: %f, grabY_: %f", y, grabY_);
        if(pushed_ && totalHeight_ > maxHeight_ && Math::abs(y - scrollY_ - grabY_) > 10) {
            //debug("dbg 4");
            scrolled_ = true;
            scrollY_ = y - grabY_;
            if(scrollY_ + totalHeight_ < maxHeight_) {
                scrollY_ = maxHeight_ - totalHeight_;
            } else if(scrollY_ > 0) {
                scrollY_ = 0;
            }
            return true;
        }
        //debug("dbg 5");
    }
    return false;
}

bool Listbox1::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

Listbox1::Item Listbox1::selectedItem() const noexcept {
    if(selectedLineNr_ >= 0 && (int)items_.size() > selectedLineNr_) {
        return items_[selectedLineNr_];
    }
    return Listbox1::Item();
}
