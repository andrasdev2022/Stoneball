#include "ApplicationMenu.h"
#include "MediaData.h"
#include "Bitmap565.h"
#include "BitmapIds.h"
#ifndef ANDROID_NDK
#include "mock.h"
#endif
#include <string.h>
#include "CommonUtil.h"
#include "Selectionbox.h"
#include "TouchEventHandler.h"
#include "Application.h"
#include "Global.h"
#include "IdGenerator.h"
#include "Memory.h"

ApplicationMenu::ApplicationMenu(MenuType menuType)
: menuType_(menuType)
, id_(IdGenerator::generateId()) {

}

void ApplicationMenu::add(MenuInterfaceSPtr subMenu) noexcept {
    subMenu->parent() = shared_from_this();
    children_.emplace(subMenu->menuType(), subMenu);
}


bool ApplicationMenu::drawBackground(MediaData* data, AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!data) {return false;}

    memset(pixels, 0, info->height * info->stride);
    //const uint16_t color = Bitmap565::make565(0xc6, 0x92, 0x42);
    //Memory::memset((uint16_t*)pixels, color, info->width * info->height);
    xOffset_ = ((int)info->width - (int)data->width()) / 2;
    yOffset_ = ((int)info->height - (int)data->height()) / 2;
    Rect srcRect(xOffset_, yOffset_, data->width(), data->height()), dstRect(0, 0, info->width, info->height);
    Bitmap565::copyIntersection((uint16_t*)pixels, dstRect, info->width, data->data(), srcRect, data->width(), true);
    return true;
}

MenuInterfaceSPtr ApplicationMenu::findMenu(MenuType t) noexcept {
    foundItem_ = nullptr;
    MenuInterfaceSPtr p = shared_from_this();
    while(p->parent()) {p = p->parent();}

    if(p->menuType() == t) {return p;}

    findMenu(t, p);
    return foundItem_;
}

bool ApplicationMenu::findMenu(MenuType t, MenuInterfaceSPtr& item) noexcept {
    auto it = item->children().find(t);
    if(it != item->children().end()) {
        foundItem_ = it->second;
        return true;
    }

    for(auto it = item->children().begin(); it != item->children().end(); ++it) {
        if(findMenu(t, it->second)) {return true;}
    }
    return false;
}

void ApplicationMenu::initSelectionboxForTime() noexcept {
    selectionboxForTimeOffsetUpdated_ = false;
    selectionboxForTime_ = new Selectionbox(0, 0);
    selectionboxForTime_->setX(233_ZRD);
    selectionboxForTime_->setY(158_ZRD);
    selectionboxForTime_->setWidth(183_ZRD);
    selectionboxForTime_->setHeight(20_ZRD);
    //debug("ApplicationMenu::initSelectionboxForTime: DBG 1 selectionboxForTime_->x(): %f", selectionboxForTime_->x());
    std::wstring label;
    for(int i = 5; i <= 60; i += 5) {
        label = std::to_wstring(i) + L" " + (*Application::translator())(TranslateInterface::Key::TRA__MINUTES);
        selectionboxForTime_->addItem(Selectionbox::Item{label, std::to_wstring(i)});
    }
    selectionboxForTime_->setName("MINUTES");
    selectionboxForTime_->setSelectedLineNr(lastSelectedLineNr_);
    auto callback = std::bind(&ApplicationMenu::selectionboxCallbackForTime, this, std::placeholders::_1, std::placeholders::_2);
    selectionboxForTime_->addSelectionCallback(callback);
    Application::touchEventHandler()->add(selectionboxForTime_);
    Application::showWidget(selectionboxForTime_);
    //debug("ApplicationMenu::initSelectionboxForTime: DBG 2 selectionboxForTime_->x(): %f", selectionboxForTime_->x());
}

bool ApplicationMenu::notify([[maybe_unused]]ObserverTouchEventSubject* subject) noexcept {
    return false;
}

void ApplicationMenu::selectionboxCallbackForTime([[maybe_unused]]Selectionbox* selectionbox, int selectedLineNr) noexcept {
    if(selectedLineNr == -1) return;
    lastSelectedLineNr_ = selectedLineNr;
}

void ApplicationMenu::drawMenuItem(BitmapId id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept {
    drawMenuItem((int)id, info, pixels, x, y);
}

void ApplicationMenu::drawMenuItem(int id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept {
    MediaData* data = bitmapData(id, false);
    int menuItemWidth = bitmapWidth(id);
    int menuItemHeight = bitmapHeight(id);
    Rect rect(0, 0, menuItemWidth, menuItemHeight);
    uint16_t* dst = (uint16_t*)pixels + info->width * (yOffset_ + y)  + xOffset_ + x;

    Bitmap565::copyIntersection(dst, rect, info->width, data->data(), rect, menuItemWidth, true);
}

void ApplicationMenu::drawRareMatrix(BitmapId id, AndroidBitmapInfo* info, void* pixels, int x, int y) noexcept {
    MediaData* data = bitmapData(id, false);
    Rect rect(0, 0, bitmapWidth(id), bitmapHeight(id));
    uint16_t* dst = (uint16_t*)pixels + info->width * y + x;
    Bitmap565::copyIntersectionRarematrix(dst, rect, info->width, data->data(), rect);
}

bool ApplicationMenu::clickTest(ObserverTouchEventSubject* subject, int x1, int x2, int y1, int y2) const noexcept {
    return subject->x() >= xOffset_ + x1 && subject->x() <= xOffset_ + x2 && subject->y() >= yOffset_ + y1 && subject->y() <= yOffset_ + y2;
}

