#include "HeroStraightWalkStrategy.h"
#include "Application.h"
#include "Environment.h"
#include "Tileset.h"
#include "AStar.h"
#include "Knight.h"
#include "MediaData.h"

HeroStraightWalkStrategy::HeroStraightWalkStrategy(double targetX, double targetY)
: Strategy(nullptr)
, lastShootTime_(-1.)
, targetX_(targetX)
, targetY_(targetY)
, btnUp_(0, 0, GameObject::Type::GOT_ARROW_UP)
, btnDown_(0, 0, GameObject::Type::GOT_ARROW_DOWN)
, btnLeft_(0, 0, GameObject::Type::GOT_ARROW_LEFT)
, btnRight_(0, 0, GameObject::Type::GOT_ARROW_RIGHT) {
}

void HeroStraightWalkStrategy::quit() noexcept {
    if(btnLast_) {
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_UP);
        btnLast_ = nullptr;
    }
    for(Option o : options_) {
        switch(o) {
        case Option::SIT_DOWN_AT_THE_END:
            knight_->sitDown();
            break;
        case Option::ACTIVATE_NEXT_TURN:
            // the Environment::finishGoalKick set the active knight id to max in order to avoid the button events
            // now we have to set it back before the next turn
            Application::environment()->setActiveKnightId(knight_->id());
            Application::nextTurn();
            break;
        case Option::STOP_AT_THE_END:
            knight_->getUp();
            // getUp calls the setStrategy(nullptr);
            return;
        default:
            debug("ERROR in HeroStraightWalkStrategy::quit: invalid option!");
        }
    }
    if(knight_->strategy()) {
        knight_->setStrategy(nullptr);
    }
}

void HeroStraightWalkStrategy::doActions(Knight* knight) noexcept {
    knight_ = knight;
    if(abs(targetX_ - knight->x()) < 5._ZRD && abs(targetY_ - knight->y()) < 5._ZRD) {
        knight->setX(targetX_);
        knight->setY(targetY_);
        quit();
        return;
    }

    if(targetX_ < knight->x()) {
        btnLast_ = &btnLeft_;
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
    } else if(targetX_ > knight->x()) {
        btnLast_ = &btnRight_;
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
    } else if(targetY_ < knight->y()) {
        btnLast_ = &btnUp_;
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
    } else if(targetY_ > knight->y()) {
        btnLast_ = &btnDown_;
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
    }
}

Strategy<Knight>* HeroStraightWalkStrategy::copy() const noexcept {
    return new HeroStraightWalkStrategy(*this);
}

void HeroStraightWalkStrategy::addOption(Option o) noexcept {
    options_.push_back(o);
}
