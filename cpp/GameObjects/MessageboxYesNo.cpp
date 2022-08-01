#include "MessageboxYesNo.h"
#include "Screen.h"
#include "BitmapIds.h"
#include "Bitmap565.h"
#include "Application.h"
#include "MediaData.h"
#include "Global.h"
#include "ObserverTouchEventSubject.h"
#include "TouchEventHandler.h"

MessageboxYesNo::MessageboxYesNo()
: BlueTriangle(0, 0, GameObject::Type::GOT_MESSAGEBOXYESNO)
{
    width_ = 200_ZRD;
    height_ = 90_ZRD;
}

MessageboxYesNo::MessageboxYesNo(Type type)
: BlueTriangle(0, 0, GameObject::Type::GOT_MESSAGEBOXYESNO)
, type_(type)
{
    width_ = 200_ZRD;
    height_ = 90_ZRD;
}

BlueTriangle* MessageboxYesNo::copy() const noexcept {
    return new MessageboxYesNo(*this);
}

double MessageboxYesNo::z() const noexcept {
    return 2500. + GameObject::z();
}

// GameObject
void MessageboxYesNo::update(double now) noexcept {
    BlueTriangle::update(now);
}

void MessageboxYesNo::drawBackground(Screen* screen, BitmapId bitmapId, int x, int y) noexcept {
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

void MessageboxYesNo::draw(Screen* screen) noexcept {
    x_ = screenPosX() + (screen->width() - width_) / 2.;
    y_ = screenPosY() + (screen->height() - height_) / 2;
    //if(Application::activeMenu() == MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
    if(type_ == Type::QUIT_PUZZLE) {
        if(Application::language() == TranslateInterface::Language::ENGLISH) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_PUZZLE_ENG, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_PUZZLE_HUN, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_PUZZLE_BUL, x_, y_);
        }
    } else if(type_ == Type::QUIT_MATCH) {
        if(Application::language() == TranslateInterface::Language::ENGLISH) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_ENG, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_HUN, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_QUIT_THE_MATCH_BUL, x_, y_);
        }
    } else if(type_ == Type::RELOAD_PUZZLE) {
        if(Application::language() == TranslateInterface::Language::ENGLISH) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RELOAD_THE_PUZZLE_ENG, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RELOAD_THE_PUZZLE_HUN, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RELOAD_THE_PUZZLE_BUL, x_, y_);
        }
    } else if(type_ == Type::RESTART_MATCH) {
        if(Application::language() == TranslateInterface::Language::ENGLISH) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RESTART_THE_MATCH_ENG, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RESTART_THE_MATCH_HUN, x_, y_);
        } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
            drawBackground(screen, NATIVE_BITMAP_MESSAGEBOX_RESTART_THE_MATCH_BUL, x_, y_);
        }
    }
}

bool MessageboxYesNo::onTouchEvent(int action, double x, double y) noexcept {
    x += screenPosX();
    y += screenPosY();

    //if(x < x_ || y < y_ || x > x_ + width_ || y > y_ + height_) return false;

    bool yes = false;
    bool no = false;
    if(x_ + 55_ZRD <= x && y_ + 53_ZRD <= y && x <= x_ + 97_ZRD && y <= y_ + 79_ZRD) {
        yes = true;
    } else if(x_ + 102_ZRD <= x && y_ + 53_ZRD <= y && x <= x_ + 144_ZRD && y <= y_ + 79_ZRD) {
        no = true;
    }
    /*
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
    }*/

    if(action == ACTION_DOWN) {
        if(yes || no) {
            pushed_ = true;
            return true;
        }
    } else if(action == ACTION_UP) {
        if(yes || no) {
            if(pushed_) {
                debug("yes or no is pushed");
                pushed_ = false;
                Application::touchEventHandler()->remove(this);
                die();
                for(auto& v : pushButtonCallbacks_) {
                    if(yes) {
                        v(this, Button::YES);
                    } else if(no) {
                        v(this, Button::NO);
                    }
                }
            }
            return true;
        }
    } else if(action == ACTION_MOVE) {
    }

    // this is a modal dialog, nothing else should receive touch events
    return true;
}

bool MessageboxYesNo::notify(ObserverTouchEventSubject* subject) noexcept {
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

