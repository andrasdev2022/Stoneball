#include "Button.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include "ObserverTouchEventSubject.h"
#include "Application.h"
#include "Algorithms.h"
#include "Tileset.h"
#include "World.h"
#include "Screen.h"
#include "MediaData.h"
#include "MultiplayerWorld.h"
#include "TilesetCardHandler.h"
#include "MenuTouchEvent.h"

using namespace Algorithms;

Button::Button(double x, double bottomY, GameObject::Type type) noexcept
: GameObject(x, bottomY, type)
, world_(nullptr)
{
    if(type == GameObject::Type::GOT_ARROW_LEFT) {
        direction_ = Vector{-1., 0.};
    } else if(type == GameObject::Type::GOT_ARROW_RIGHT) {
        direction_ = Vector{1., 0.};
    } else if(type == GameObject::Type::GOT_ARROW_UP) {
        direction_ = Vector{0., -1.};
    } else if(type == GameObject::Type::GOT_ARROW_DOWN) {
        direction_ = Vector{0., 1.};
    } else if(anyOf(type, GameObject::Type::GOT_REFRESH,
                          GameObject::Type::GOT_BACK,
                          GameObject::Type::GOT_UNDO,
                          GameObject::Type::GOT_NEXT_TURN,
                          GameObject::Type::GOT_PASS,
                          GameObject::Type::GOT_EMOTE_BUTTON)) {
        direction_ = Vector{0., 0.};
    }
    init();
    transparency_ = 0;
    skipEveryNthLine = 0;
    //width_ = bitmapWidth(frameIndex_);
}

bool Button::notify(ObserverTouchEventSubject* subject) noexcept {
    if(viewOnlyEmotesButton()) return false;
    return onTouchEvent(subject->action(), subject->x(), subject->y());
}

bool Button::checkWorldPtr() noexcept {
    if(world_) {
        if(!Application::game() || !Application::game()->worldListSize()) return false;
        setWorldPtr(Application::latestWorld());
    }
    return true;
}

bool Button::onTouchEvent(int action, double x, double y) noexcept {
    if(action == ACTION_MOVE) return false;
/*
    std::cout << "x: " << x << ", "
              << "y: " << y << ", "
              << "x_: " << x_ << ", "
              << "y_: " << y_ << ", "
              << "width_: " << width_ << ", "
              << "height_: " << height_ << ", "
              << "action: " << str(static_cast<Event>(action)) << ", "
              << "type_: " << str(type_) << ", "
              << "\n";

    if(type_ == GameObject::Type::GOT_BACK) {
        std::cout << std::string(40, '-') << "\n";
    }
*/
    // középpont: C(u,v); (x - u) * (x - u) + (y - v) * (y - v) = r * r
    Vector C{(double)(x_ + width_ / 2.), (double)(y_ + height_ / 2.)};
    double result = (x - C.x) * (x - C.x) + (y - C.y) * (y - C.y) - width_ * width_ / 4.0f;
    if(result < 0.) {
        //std::cout << "Button::onTouchEvent result OK\n";
        if(type_ == GameObject::Type::GOT_REFRESH) {
            if(action == ACTION_DOWN) {
                pushed_ = true;
                //return true;
            } else if(action == ACTION_UP && pushed_) {
                if(Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_FIND_GAME) {
                    MenuTouchEvent::sendRoomListRequest();
                } else {
                    Application::restartLevel();
                }
                pushed_ = false;
                //return true;
            }
        } else if(type_ == GameObject::Type::GOT_BACK) {
            if(action == ACTION_DOWN) {
                pushed_ = true;
                //return true;
            } else if(action == ACTION_UP && pushed_) {
                Application::goUpInTheMenu();
                pushed_ = false;
                //return true;
            }
        } else if(type_ == GameObject::Type::GOT_UNDO) {
            debug("Button::onTouchEvent");
            if(action == ACTION_DOWN) {
                pushed_ = true;
                //return true;
            } else if(action == ACTION_UP && pushed_) {
                Application::undoLastStep();
                pushed_ = false;
                //return true;
            }
        } else if(type_ == GameObject::Type::GOT_EMOTE_BUTTON) {
            debug("Button::onTouchEvent");
            if(action == ACTION_DOWN) {
                pushed_ = true;
                Application::showEmotesBubble();
            } else if(action == ACTION_UP && pushed_) {
                //Application::hideEmotesBubble();
                pushed_ = false;
                //return true;
            }
        } else if(type_ == GameObject::Type::GOT_NEXT_TURN || type_ == GameObject::Type::GOT_PASS) {
            if(action == ACTION_DOWN) {
                pushed_ = true;
                //return true;
                //debug("nextTurn Pushed!!!!\n");
            } else if(action == ACTION_UP && pushed_) {
                //debug("nextTurn Released!!!!\n");
                // The end turn button should not work if the player's move histor is empty or he moved back to the start position
                // In multiplayer mode, the history content is not syncronized between client and server thus the history size check is done here, but
                // the second condition must be done on the server
                auto tileset = Application::tileset();
                if(checkWorldPtr() && !tileset->gameOver() && (!world_->multiplayer() || world_->onButtonEvent(this, ButtonState::BS_UP))) {
                    if(!world_->multiplayer()) {
                        Application::nextTurn();
                    }
                }
                pushed_ = false;
                //return true;
            }
        } else {
            /*return */handleArrowButtons(action);
        }
        // touch on a button is always true
        hideCards();
        return true;
    } else if(pushed_) {
        if(anyOf(type_, GameObject::Type::GOT_ARROW_RIGHT, GameObject::Type::GOT_ARROW_LEFT, GameObject::Type::GOT_ARROW_UP, GameObject::Type::GOT_ARROW_DOWN)) {
            return handleArrowButtons(ACTION_UP);
        }
        pushed_ = false;
    }
    //std::cout << "Button::onTouchEvent is returning with false\n";
    return false;
}

bool Button::handleArrowButtons(int action) noexcept {
    //debug("DBG Button::handleArrowButtons action: %d, type: %d", action, type_);
    if(action == ACTION_DOWN || action == ACTION_POINTER_DOWN) {
        //debug("DBG Button::onTouchEvent 2 action: %d, x: %f, y: %f", action, x, y);
        if(checkWorldPtr() && world_) {
            pushed_ = world_->onButtonEvent(this, ButtonState::BS_DOWN);
            //std::cout << "Button::handleArrowButtons return trues 1\n";
            if(pushed_) hideCards();
            return pushed_;
        }
    } else if(action == ACTION_UP || action == ACTION_POINTER_UP) {
        //debug("DBG Button::onTouchEvent 3 action: %d, x: %f, y: %f", action, x, y);
        if(checkWorldPtr() && world_ && world_->onButtonEvent(this, ButtonState::BS_UP)) {
            pushed_ = false;
            //std::cout << "Button::handleArrowButtons returns true 2\n";
            hideCards();
            return true;
        }
    }
    return false;
}

void Button::update(double now) noexcept {
    updateFrame(now, skinMin_, skinMax_);
}

bool Button::viewOnlyEmotesButton() noexcept {
    return (type() == GameObject::Type::GOT_EMOTE_BUTTON && Application::environment() && Application::tileset() && Application::tileset()->viewOnly());
}

void Button::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(viewOnlyEmotesButton()) return;
    GameObject::draw(info, pixels);
    if(label_ == TranslateInterface::Key::LAST_ONE) return;

    auto& f12px = Application::font12px();
    (*f12px).setColor(0x29, 0x20, 0x18);

    auto& tr = Application::translator();
    auto text = (*tr)(label_);
    int textWidth = (*f12px).width(text);
    int textHeight = (*f12px).height(text);

#ifdef WIN32
    int xpos = floor(x_ + (width_ - textWidth) / 2) + 1_ZRD;
    int ypos = y_ + height_ - 2 * textHeight + 9_ZRD;
#else
    int xpos = floor(x_ + (width_ - textWidth) / 2) - 1_ZRD;
    int ypos = y_ + height_ - 2 * textHeight + 6_ZRD;
#endif
    Screen screen(0, 0, screenWidth(), screenHeight());
    screen.setContentPtr(Application::screen()->content());

    (*f12px).draw(&screen, xpos, ypos, text);
}

const GameObjectDescriptor& Button::god() const noexcept {
    return GameObjectDescriptor::find(type_, State::Type::ST_IDLE);
}

void Button::hideCards() noexcept {
    if(checkWorldPtr() && world_ && Application::environment() && Application::tileset()
    && !world_->multiplayer() && Application::tileset()->tilesetCardHandler()) {
        Application::tileset()->tilesetCardHandler()->hideAllCards();
    }
}

void Button::setType(GameObjectInterface::Type type) noexcept {
    type_ = type;
    y_ += contentBottom() - y_;
    init();
}
