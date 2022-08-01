#include "LevelsMenu.h"
#include "BitmapIds.h"
#include "MediaData.h"
#include "LevelFactory.h"
#include "EnumClassOp.h"
#include "RPGBgElement.h"
#include "MenuTouchEvent.h"
#include "Application.h"
#include "Screen.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include  <array>
#include "Bitmap565.h"
#include "Collision.h"

static constexpr int pageSize = 15;

LevelsMenu::LevelsMenu()
: ApplicationMenu(MenuType::LEVELS) {
}

void LevelsMenu::update(double now) noexcept {
    initHoles();
    for(size_t i = 0; i < holesForLevels_.size(); ++i) {
        if(Application::levelStatuses()[i] && holesForLevels_[i]->skinMin() != NATIVE_BITMAP_HOLE_DARK_01) {
            holesForLevels_[i]->changeBitmapId(NATIVE_BITMAP_HOLE_DARK_01);
        }
        holesForLevels_[i]->update(now);
    }
}

void LevelsMenu::drawArrow(AndroidBitmapInfo* info, void* pixels, bool left) noexcept {
    if(currentPage_ == 0 && left) {return;}

    EnumClassOp i(LevelFactory::GameLevel::LEVEL_1);
    if(!left && i + (currentPage_ + 1) * pageSize >= LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1) {return;}

    MediaData* arrowData = bitmapData(NATIVE_BITMAP_YELLOW_LEVEL_PAGER, !left);

    int x = xOffset_ + 104_ZRD;
    int y = yOffset_ + 136_ZRD;

    if(!left) {x = xOffset_ + 447_ZRD;}

    Rect srcRect(x, y, arrowData->width(), arrowData->height()), dstRect(0, 0, info->width, info->height);
    Bitmap565::copyIntersectionRarematrix((uint16_t*)pixels, dstRect, info->width, arrowData->data(), srcRect);
}

void LevelsMenu::drawArrows(AndroidBitmapInfo* info, void* pixels) noexcept {
    drawArrow(info, pixels, true);
    drawArrow(info, pixels, false);
}

bool LevelsMenu::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(!drawBackground(bitmapData(NATIVE_BITMAP_MENU_BACKGROUND, false), info, pixels)) {return false;}
    drawArrows(info, pixels);

    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawMenuItem(NATIVE_BITMAP_LEVELS_MENU_ENG_TITLE, info, pixels, 237_ZRD, 73_ZRD);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawMenuItem(NATIVE_BITMAP_LEVELS_MENU_HUN_TITLE, info, pixels, 230_ZRD, 73_ZRD);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawMenuItem(NATIVE_BITMAP_LEVELS_MENU_BUL_TITLE, info, pixels, 254_ZRD, 73_ZRD);
    }

    Screen screenL(0, 0, screenWidth(), screenHeight());
    screenL.setContentPtr((uint16_t*)pixels);

    auto& font24px = Application::font24px();
    font24px->setColor(0xdd, 0xc6, 0x9f);

    initHolesOffsets();

    const size_t startHole = currentPage_ * pageSize;
    for(size_t i = startHole; i < startHole + pageSize && i < holesForLevels_.size(); ++i) {
        holesForLevels_[i]->draw(info, pixels);
        auto txt = std::to_wstring(i + 1);
        double x = holesForLevels_[i]->x() + (holesForLevels_[i]->width() - font24px->width(txt)) / 2.;
        font24px->draw(&screenL, x, holesForLevels_[i]->y() - 15_ZRD, txt);
        if(Application::levelStatuses()[i] == 0) continue;

        if(Application::levelStatuses()[i] <= LevelFactory::minimumPushCount[i]) {
            drawRareMatrix(NATIVE_BITMAP_3STARS_3, info, pixels, holesForLevels_[i]->x() - 2_ZRD, holesForLevels_[i]->y() + 2_ZRD);
        } else if((float)Application::levelStatuses()[i] <= LevelFactory::minimumPushCount[i] * 1.25f) {
            drawRareMatrix(NATIVE_BITMAP_3STARS_2, info, pixels, holesForLevels_[i]->x() - 2_ZRD, holesForLevels_[i]->y() + 2_ZRD);
        } else if((float)Application::levelStatuses()[i] <= LevelFactory::minimumPushCount[i] * 1.5f) {
            drawRareMatrix(NATIVE_BITMAP_3STARS_1, info, pixels, holesForLevels_[i]->x() - 2_ZRD, holesForLevels_[i]->y() + 2_ZRD);
        } else {
            drawRareMatrix(NATIVE_BITMAP_3STARS_0, info, pixels, holesForLevels_[i]->x() - 2_ZRD, holesForLevels_[i]->y() + 2_ZRD);
        }
    }
    offsetsInitialized_ = true;
    return true;
}

void LevelsMenu::initHoles() noexcept {
    MenuTouchEvent* menuTouchEvent = Application::menuTouchEvent();

    if(!holesForLevels_.size() && menuTouchEvent) {
        const int startHole = currentPage_ * pageSize;
        EnumClassOp i(LevelFactory::GameLevel::LEVEL_1);
        i = i + startHole;
        for(; (int)i < startHole + 5 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1) * 30_ZRD + ((int)i - startHole) * 32_ZRD, 122_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }

        for(; (int)i < startHole + 10 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1 - 5) * 30_ZRD + ((int)i - startHole - 5) * 32_ZRD, 180_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }

        for(; (int)i < startHole + 15 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1 - 10) * 30_ZRD + ((int)i - startHole - 10) * 32_ZRD, 240_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }

        for(; (int)i < startHole + 20 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1 - 15) * 30_ZRD + ((int)i - startHole - 15) * 32_ZRD, 122_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }

        for(; (int)i < startHole + 25 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1 - 20) * 30_ZRD + ((int)i - startHole - 20) * 32_ZRD, 180_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }

        for(; (int)i < startHole + 30 && i < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1; ++i) {
            holesForLevels_.push_back(std::shared_ptr<RPGBgElement>(new RPGBgElement(113_ZRD + ((int)i - startHole + 1 - 25) * 30_ZRD + ((int)i - startHole - 25) * 32_ZRD, 240_ZRD, NATIVE_BITMAP_HOLE_RED_01)));
        }
    }
}

void LevelsMenu::initHolesOffsets() noexcept {
    if(offsetsInitialized_) return;
    for(size_t i = 0; i < holesForLevels_.size(); ++i) {
        holesForLevels_[i]->setX(holesForLevels_[i]->x() + xOffset_);
        holesForLevels_[i]->setY(holesForLevels_[i]->y() + yOffset_);
    }
}

bool LevelsMenu::arrowClick(ObserverTouchEventSubject* subject) noexcept {
    MediaData* arrowData = bitmapData(NATIVE_BITMAP_YELLOW_LEVEL_PAGER, false);

    Rect leftArrow(xOffset_ + 104_ZRD, yOffset_ + 136_ZRD, arrowData->width(), arrowData->height());
    if(currentPage_ > 0 && Collision::insideRect(subject->x(), subject->y(), leftArrow)) {
         --currentPage_;
        return true;
    }

    Rect rightArrow(xOffset_ + 447_ZRD, yOffset_ + 136_ZRD, arrowData->width(), arrowData->height());
    if(Collision::insideRect(subject->x(), subject->y(), rightArrow)) {
        EnumClassOp i(LevelFactory::GameLevel::LEVEL_1);
        if(i + (currentPage_ + 1) * pageSize < LevelFactory::GameLevel::LEVEL_PLAY_WITH_FRIEND_1) {
            ++currentPage_;
        }
        return true;
    }
    return false;
}

bool LevelsMenu::notify(ObserverTouchEventSubject* subject) noexcept {
    if(arrowClick(subject)) {return true;}

    for(size_t i = 0; i < holesForLevels_.size(); ++i) {
        auto& hole = holesForLevels_[i];
        if(subject->x() >= hole->x() && subject->x() <= hole->x() + hole->width() &&
           subject->y() >= hole->y() - 2 * hole->height() && subject->y() <= hole->y() + hole->height() &&
           i >= (size_t)currentPage_ * pageSize)
        {
            //debug("hole %d selected", i);
            Application::setActiveMenu(MenuInterface::MenuType::SINGLEPLAYER_RUNNIG);
            Application::setGameLevel(static_cast<LevelFactory::GameLevel>(i));
            return true;
        }
    }
    return false;
}
