#include "Game.h"
#include "Global.h"
#include "World.h"
#include "Button.h"
#include "MediaData.h"
#include "Application.h"
#include "Algorithms.h"
#include "WorldTouchEvent.h"
#include "Tileset.h"

using namespace Algorithms;

void Game::wakeUp() noexcept {
    double now = now_ms();
    if(worldList_.size() != 0) {
        latestWorld()->wakeUp(now);
    }

    std::for_each(buttons_.begin(), buttons_.end(), [now](Button*b){b->wakeUp(now);});
}

void Game::initButtons() noexcept {
    worldTouchEvent_ = std::make_unique<WorldTouchEvent>();

    // these must be bottom aligned
    arrowButtonRight_ = std::make_unique<Button>(64_ZRD, screenHeight(), GameObject::Type::GOT_ARROW_RIGHT);
    arrowButtonUp_ = std::make_unique<Button>(screenWidth() - 64_ZRD, screenHeight() - 64_ZRD, GameObject::Type::GOT_ARROW_UP);
    arrowButtonDown_ = std::make_unique<Button>(screenWidth() - 64_ZRD, screenHeight(), GameObject::Type::GOT_ARROW_DOWN);
    arrowButtonLeft_ = std::make_unique<Button>(0_ZRD, screenHeight(), GameObject::Type::GOT_ARROW_LEFT);
    nextTurnButton_ = std::make_unique<Button>(screenWidth() - 64_ZRD, screenHeight() - 128_ZRD, GameObject::Type::GOT_PASS);
    nextTurnButton_->setLabel(TranslateInterface::Key::TRA__LABEL_PASS);
    emotesBubble_ = std::make_unique<EmotesBubble>(397_ZRD, nextTurnButton_->y());

    buttons().clear();
    buttons().push_back(arrowButtonRight_.get());
    buttons().push_back(arrowButtonUp_.get());
    buttons().push_back(arrowButtonDown_.get());
    buttons().push_back(arrowButtonLeft_.get());
}

void Game::changeButtonsAccordingToPlaymode() noexcept {
    auto it = std::find_if(buttons().begin(), buttons().end(), [&](const Button* i){return i == nextTurnButton_.get();});
    if(Application::playMode() == Application::PlayMode::TURNBASED) {
        if(it == buttons().end()) {
            buttons().push_back(nextTurnButton_.get());
        }
    } else {
        if(it != buttons().end()) {
            buttons().erase(it);
        }
    }
}

void Game::updateWorldOfButtons(WorldUPtr& world) noexcept {
    std::for_each(buttons().begin(), buttons().end(), [&world](Button*b){b->setWorldPtr(world);});
    worldTouchEvent_->setWorldPtr(world);
    emotesBubble_->setWorldPtr(world);
}

void Game::resetWorldTouchEvent() noexcept {
    if(worldTouchEvent_) {
        WorldUPtr world;
        worldTouchEvent_->setWorldPtr(world);
    }
    buttons().clear();
}

bool Game::showNextTurnButton() noexcept {
    if(Application::tileset()->viewOnly()) return false;
    auto it = std::find_if(buttons().begin(), buttons().end(), [&](const Button* i){return i->id() == nextTurnButton()->id();});
    if(it == buttons().end()) {
        buttons().push_back(nextTurnButton());
        return true;
    }
    return false;
}

Button* Game::hideNextTurnButton() noexcept {
    Button* retV = nullptr;
    auto it = std::find_if(buttons().begin(), buttons().end(), [&](const Button* i){return i->id() == nextTurnButton()->id();});
    if(it != buttons().end()) {
        retV = *it;
        buttons().erase(it);
    }
    return retV;
}

void Game::changeNextTurnButtonLabel(TranslateInterface::Key key) noexcept {
    if(nextTurnButton()) {
        nextTurnButton()->setLabel(key);
        if(key == TranslateInterface::Key::TRA__LABEL_PASS) {
            nextTurnButton()->setType(GameObjectInterface::Type::GOT_PASS);
        } else if(key == TranslateInterface::Key::TRA__LABEL_NEXT_TURN) {
            nextTurnButton()->setType(GameObjectInterface::Type::GOT_NEXT_TURN);
        }
    }
}

void Game::updateANewCopyOfTheLastestWorld(double now) noexcept {
    WorldUPtr world = std::unique_ptr<World>(latestWorld()->copy());
    updateWorldOfButtons(world);
    auto retv = worldList().emplace(worldList().rbegin()->first + 1, std::move(world));
    if(!retv.second) {
        debug("ERROR: worldList().emplace failure 4");
    }
    latestWorld()->update(now);
}

void Game::hideArrowButtons() noexcept {
    for(auto id : {arrowButtonRight()->id(), arrowButtonUp()->id(), arrowButtonDown()->id(), arrowButtonLeft()->id()}) {
        auto it = std::find_if(buttons().begin(), buttons().end(), [&](const Button* i){return i->id() == id;});
        if(it != buttons().end()) {
            buttons().erase(it);
        }
    }
}
