#include "StateKnightIdle.h"
#include "StateKnightRun.h"
#include "stoneball.h"
#include "StateSubject.h"
#include "GameObjectDescriptor.h"
#include "Knight.h"
#include "MediaData.h"
#include "Math_.h"
#include "Application.h"
#include "Tileset.h"
#include "ButtonEventListener.h"
#include "Visitor.h"
#include "World.h"
#include "BitmapIds.h"
#include "TilesetCardHandler.h"
#include "CardPushPlayer.h"

#include <math.h>
#include <iostream>

constexpr double tolerance = 1e-6;
StateKnightRun::StateKnightRun(double x, double bottomY, GameObject::Type heroType, Direction direction) noexcept
 : GameObject(x, bottomY, heroType) {

    changeDescriptor(direction);
    init();

    buttonActionType_ = GameObject::Type::COUNT;
    actionParamSpeed_ = Speed::FAST;
    startX_ = x_;
    startY_ = y_;
    stopping_ = false;
    keepRunning_ = false;
}

State* StateKnightRun::copy() const noexcept {
    return new StateKnightRun(*this);
}

void StateKnightRun::change(StateSubject* subject) noexcept {
    if(ready_) {
        //std::cout << "StateKnightRun::change ready\n";
        if(keepRunning_) {
            //std::cout << "StateKnightRun::change running fast\n";
            if(buttonActionType_ == GameObject::Type::GOT_ARROW_UP) {
                subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::NORTH));
            } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_DOWN) {
                subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::SOUTH));
            } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_RIGHT) {
                subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::EAST));
            } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_LEFT) {
                subject->setState(new StateKnightRun(x_, contentBottom(), type_, Direction::WEST));
            }
        } else {
            subject->setState(new StateKnightIdle(x_, contentBottom(), type_, direction_.x));
        }
    }
}

// This funtion keeps the hero straight, if it is necessary
void StateKnightRun::correctCoordinates() noexcept {
    Tileset* tileset = Application::tileset();
    int xDifference = tileset->testKnightXPosition(x_);
    //std::cout << "StateKnightRun::correctCoordinates xDifference: " << xDifference << "\n";
    if(Math::abs(direction_.x) < tolerance && xDifference != 0) {
        if(xDifference >= tileset->tileWidth() / 2) {
            setX(x_ + .1);
        } else {
            setX(x_ - .1);
        }
    }
    int yDifference = tileset->testKnightYPosition(y_);
    //std::cout << "StateKnightRun::correctCoordinates                               yDifference: " << yDifference << "\n";
    if(Math::abs(direction_.y) < tolerance && yDifference != 0) {
        if(yDifference >= tileset->tileHeight() / 2) {
            setY(y_ + .1);
        } else {
            setY(y_ - .1);
        }
    }
}

bool StateKnightRun::stopOnTile() noexcept {
    //std::cout << "StateKnightRun::stopOnTile begin\n";
    // We must be on a valid tile

    Tileset* tileset = Application::tileset();
    while(!stopping_ && Math::abs(x_ - startX_) / tileset->tileWidth() >= 1.) {
        if(x_ > startX_) startX_ += tileset->tileWidth();
        else if(x_ < startX_) startX_ -= tileset->tileWidth();
    }
    while(!stopping_ && Math::abs(y_ - startY_) / tileset->tileHeight() >= 1.) {
        if(y_ > startY_) startY_ += tileset->tileHeight();
        else if(y_ < startY_) startY_ -= tileset->tileHeight();
    }
    stopping_ = true;
    bool cond1 = Math::abs((int)round(x_ - startX_)) > tileset->tileWidth() && Math::abs(direction_.y) < tolerance;
    bool cond2 = Math::abs((int)round(y_ - startY_)) > tileset->tileHeight() && Math::abs(direction_.x) < tolerance;

    double tileX = tileset->convertTileX2KnightX(tileset->convertKnightX2TileX(x_ + ((direction_.x < -.5) ? tileset->tileWidth() / 2 : 0)));
    double tileY = tileset->convertTileY2KnightY(tileset->convertKnightY2TileY(y_ + ((direction_.y < -.5) ? tileset->tileHeight() / 2 : 0)));

    //debug("StateKnightRun::stopOnTile x_: %f startX_: %f y_: %f startY_: %f direction_.x: %f direction_.y: %f, tileHeight: %d, tileY: %f",
    //      x_, startX_, y_, startY_, direction_.x, direction_.y, tileset->tileHeight(), tileY);

    bool cond3 = Math::abs(direction_.x) > tolerance && Math::abs(tileX - x_) < 5._ZRD;
    bool cond4 = Math::abs(direction_.y) > tolerance && Math::abs(tileY - y_) < 5._ZRD;
    if((cond1 && cond3) || (cond2 && cond4)) {
        if(cond1) {
            double x = x_;
            if(direction_.x < -.5) x += tileset->tileWidth() / 2;
            setX(tileset->convertTileX2KnightX(tileset->convertKnightX2TileX(x)));
        } else {
            double y = y_;
            if(direction_.y < -.5) y += tileset->tileHeight() / 2;
            setY(tileset->convertTileY2KnightY(tileset->convertKnightY2TileY(y)));
            debug("New y(): %f, y: %f", this->y(), y);
        }
        //std::cout << "StateKnightRun::stopOnTile ready\n";
        ready_ = true;
        return true;
    }
    return false;
}

bool StateKnightRun::isNextTileValid(double originalX, double originalY) noexcept {
    Tileset* tileset = Application::tileset();
    int nextTileX = tileset->convertKnightX2TileX(originalX);
    int nextTileY = tileset->convertKnightY2TileY(originalY);

    // Are we at the border of the tileset? different look ahead is necessary.
    if(direction_.x > .5) ++nextTileX;
    if(direction_.y > .5) ++nextTileY;

    if(tileset->isBlockingTile(nextTileX, nextTileY, direction_)) {
        //std::cout << "Something is blocking\n";
        return false;
    }

    if(findKnightOnTheTile(nextTileX, nextTileY)) {
        // the knight can push another knight with this card
        auto tch = tileset->tilesetCardHandler();
        if(tch && tch->isPushPlayerCardActive() && tch->getActivePushPlayerCard() && tch->getActivePushPlayerCard()->amount() > 0) {
            // is this the current tile or the next one?
            // in the first case we have to check two tiles distance
            int iteration = 1;
            double absX = Math::abs(originalX - startX_);
            double absY = Math::abs(originalY - startY_);
            if(absX < .1 && absY < .1) {
                ++iteration;
            }
            if(direction_.x > .5) ++nextTileX;
            if(direction_.y > .5) ++nextTileY;

            for(int i = 0; i < iteration; ++i) {
                if(direction_.x < -.5) --nextTileX;
                if(direction_.y < -.5) --nextTileY;
            }

            if(tileset->isBlockingTile(nextTileX, nextTileY, direction_)) {
                std::cout << "Something is blocking\n";
                return false;
            }
            if(findKnightOnTheTile(nextTileX, nextTileY)) {
                return false;
            }
        } else {
            return false;
        }
    }
    return true;
}

bool StateKnightRun::findKnightOnTheTile(int nextTileX, int nextTileY) {
    class V : public Visitor {
    public:
        void visit(StateKnightRun* go) override {ptr = go;}
        GameObject* ptr{};
    };

    // No-one else (another knight) should be on the tile.
    auto& world = Application::latestWorld();
    Tileset* tileset = Application::tileset();
    for(auto& knightP : world->knightList()) {
        auto& knight = knightP.second;

        V visitor;
        knight->state()->accept(&visitor);

        if(visitor.ptr && visitor.ptr->id() == id()) {
            continue;
        }

        // This condition doesn't work on the server. Why?!
        //if(knight->state().get() == this) continue;

        int x = tileset->convertKnightX2TileX(knight->x());
        int y = tileset->convertKnightY2TileY(knight->y());

        if(nextTileX == x && nextTileY == y) {
            //debug("There is another knight on the tile");
            //debug("knight type: %s", GameObject::str(knight->type()).c_str());
            return true;
        }
    }
    return false;
}

void StateKnightRun::revetPosition() noexcept {
    Tileset* tileset = Application::tileset();

    if(direction_.x > .5) x_ = tileset->getTileX(tileset->convertKnightX2TileX(x_));
    else if(direction_.x < -.5) x_ = tileset->getTileX(tileset->convertKnightX2TileX(x_) + 1);

    if(direction_.y > .5) y_ = tileset->getTileY(tileset->convertKnightY2TileY(y_));
    else if(direction_.y < -.5) y_ = tileset->getTileY(tileset->convertKnightY2TileY(y_) + 1);
}

void StateKnightRun::update(double now) noexcept {
    //LOGI("StateKnightRun::update direction_: %f, %f", direction_.x, direction_.y);
    if(ready_) return;
    correctCoordinates();
    updateFrame(now, skinMin_, skinMax_);
    walkOrMove(now);
    if(!isNextTileValid(x_, y_)) {
        //LOGI("next tile invalid");
        revetPosition();
        ready_ = true;
    } else if(actionParamSpeed_ == Speed::ZERO || !matchButtonAndDirection()) {
        // We have to check wether the player changed the pressed arrow button during the knight's run.
        // if so, he should stop
        stopOnTile();
    }
}

bool StateKnightRun::matchButtonAndDirection() noexcept {
    if(buttonActionType_ == GameObject::Type::COUNT) {
        return true;
    } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_UP && (int)directionX() == 0  && (int)directionY() == -1) {
        return true;
    } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_DOWN && (int)directionX() == 0  && (int)directionY() == 1) {
        return true;
    } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_RIGHT && (int)directionX() == 1  && (int)directionY() == 0) {
        return true;
    } else if(buttonActionType_ == GameObject::Type::GOT_ARROW_LEFT && (int)directionX() == -1  && (int)directionY() == 0) {
        return true;
    }
    return false;
}

const GameObjectDescriptor& StateKnightRun::god() const noexcept {
    return descriptor_;
}

// ButtonEventListener
// return true: there will be a state change without ready_ = true, and the new state will
// get the event too
bool StateKnightRun::onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept {
    Unused(0, sender);

    buttonActionType_ = GameObject::Type::COUNT;
    actionParamSpeed_ = Speed::FAST;
    if(buttonState == ButtonState::BS_UP) {
        actionParamSpeed_ = Speed::ZERO;
        return true;
    }

    // Blindly following the strategy's instruction
    if(buttonState == ButtonState::BS_DOWN) {
        if(sender->type() == GameObject::Type::GOT_ARROW_UP) {
            buttonActionType_ = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_DOWN) {
            buttonActionType_ = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_RIGHT) {
            buttonActionType_ = sender->type();
            return true;
        }
        if(sender->type() == GameObject::Type::GOT_ARROW_LEFT) {
            buttonActionType_ = sender->type();
            return true;
        }
    }

    return false;
}

void StateKnightRun::changeDescriptor(Direction direction) noexcept {
    descriptor_ = GameObjectDescriptor::find(type_, State::Type::ST_RUN);
    //LOGI("StateKnightRun::changeDescriptor");
    switch(type_) {
    case GameObject::Type::GOT_KNIGHT_RED:
        switch(direction) {
        case Direction::NORTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_RED_KNIGHT_BACK_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_RED_KNIGHT_BACK_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_RED_KNIGHT_BACK_3);
            this->direction_ = Vector{0., -1.};
            break;
        case Direction::SOUTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_RED_KNIGHT_FRONT_3);
            this->direction_ = Vector{0., 1.};
            break;
        case Direction::EAST:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_RED_KNIGHT_RIGHT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_RED_KNIGHT_RIGHT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_RED_KNIGHT_RIGHT_3);
            this->direction_ = Vector{1., 0.};
            break;
        case Direction::WEST:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_RED_KNIGHT_LEFT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_RED_KNIGHT_LEFT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_RED_KNIGHT_LEFT_3);
            this->direction_ = Vector{-1., 0.};
            //LOGI("StateKnightRun::changeDescriptor west");
            break;
        default: break;
        }
        break;
    case GameObject::Type::GOT_KNIGHT_DARK:
        switch(direction) {
        case Direction::NORTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_DARK_KNIGHT_BACK_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_DARK_KNIGHT_BACK_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_DARK_KNIGHT_BACK_3);
            this->direction_ = Vector{0., -1.};
            break;
        case Direction::SOUTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_DARK_KNIGHT_FRONT_3);
            this->direction_ = Vector{0., 1.};
            break;
        case Direction::EAST:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_DARK_KNIGHT_RIGHT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_DARK_KNIGHT_RIGHT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_DARK_KNIGHT_RIGHT_3);
            this->direction_ = Vector{1., 0.};
            break;
        case Direction::WEST:
            descriptor_.setSkinStart(NATIVE_BITMAP_MALE_DARK_KNIGHT_LEFT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_MALE_DARK_KNIGHT_LEFT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_MALE_DARK_KNIGHT_LEFT_3);
            this->direction_ = Vector{-1., 0.};
            break;
        default: break;
        }
        break;
    case GameObject::Type::GOT_KNIGHT_BLONDE_FEMALE:
        switch(direction) {
        case Direction::NORTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_BACK_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_BACK_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_BACK_3);
            this->direction_ = Vector{0., -1.};
            break;
        case Direction::SOUTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_FRONT_3);
            this->direction_ = Vector{0., 1.};
            break;
        case Direction::EAST:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_RIGHT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_RIGHT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_RIGHT_3);
            this->direction_ = Vector{1., 0.};
            break;
        case Direction::WEST:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_LEFT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_LEFT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_BLONDE_KNIGHT_LEFT_3);
            this->direction_ = Vector{-1., 0.};
            break;
        default: break;
        }
        break;
    case GameObject::Type::GOT_KNIGHT_GREEN_FEMALE:
        switch(direction) {
        case Direction::NORTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_BACK_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_BACK_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_BACK_3);
            this->direction_ = Vector{0., -1.};
            break;
        case Direction::SOUTH:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_FRONT_3);
            this->direction_ = Vector{0., 1.};
            break;
        case Direction::EAST:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_RIGHT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_RIGHT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_RIGHT_3);
            this->direction_ = Vector{1., 0.};
            break;
        case Direction::WEST:
            descriptor_.setSkinStart(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_LEFT_1);
            descriptor_.setSkinMin(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_LEFT_1);
            descriptor_.setSkinMax(NATIVE_BITMAP_FEMALE_GREEN_KNIGHT_LEFT_3);
            this->direction_ = Vector{-1., 0.};
            break;
        default: break;
        }
        break;
    default: break;
    }
}

void StateKnightRun::keepRunning(bool run) {
    //std::cout << "StateKnightRun::keepRunning " << run << "\n";
    keepRunning_ = run;
}
