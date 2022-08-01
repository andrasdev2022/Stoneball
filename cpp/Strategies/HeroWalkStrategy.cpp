#include "HeroWalkStrategy.h"
#include "Application.h"
#include "Tileset.h"
#include "AStar.h"
#include "Knight.h"
#include "WorldTouchEvent.h"

HeroWalkStrategy::HeroWalkStrategy(int targetX, int targetY)
: Strategy(nullptr)
, lastShootTime_(-1.)
, targetX_(targetX)
, targetY_(targetY)
, btnUp_(0, 0, GameObject::Type::GOT_ARROW_UP)
, btnDown_(0, 0, GameObject::Type::GOT_ARROW_DOWN)
, btnLeft_(0, 0, GameObject::Type::GOT_ARROW_LEFT)
, btnRight_(0, 0, GameObject::Type::GOT_ARROW_RIGHT)
{
}

void HeroWalkStrategy::quit() {
    if(btnLast_) {
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_UP);
        btnLast_ = nullptr;
    }
    knight_->setStrategy(nullptr);
}

bool HeroWalkStrategy::findNextTileForHero() {
    bool stop = false;
    for(size_t y = 0; y < aStarResult_[stepIndex_].size() && !stop; ++y) {
        for(size_t x = 0; x < aStarResult_[stepIndex_][y].size() && !stop; ++x) {
            if(aStarResult_[stepIndex_][y][x] == Tileset::redKnight) {
                nextX_ = x;
                nextY_ = y;
                stop = true;
            }
        }
    }
    return stop;
}

void HeroWalkStrategy::findPath() {
    MazeStar maze;
    maze = tileset_->getMazeVector(knight_, heroTileX_, heroTileY_);
    // set the target position empty
    maze.setValue(targetX_, targetY_, Tileset::emptyField);

    AStar aStar(targetX_, targetY_);
    aStar << maze;
    if(aStar.search(false)) {
        // Saving the result
        aStarResult_ = aStar.getResult();
        stepIndex_ = aStarResult_.size() - 2; // next step
    } else {
        // We cannot fint the way, let's pop a questionmark
        WorldTouchEvent wte;
        wte.setWorldPtr(Application::latestWorld());
        wte.popQuestionmarksAboveKnights();
        knight_->setStrategy(nullptr);
    }
}

bool HeroWalkStrategy::waitForHeroWalk() {
    double heroHeadingX = knight_->x();
    double heroHeadingY = knight_->y();
    if(knight_->directionX() > .5) {
        heroHeadingX += tileset_->tileWidth() * .9;
    }
    if(knight_->directionY() > .5) {
        heroHeadingY += tileset_->tileHeight() * .9;
    }

    int heroHeadingTileX = tileset_->convertKnightX2TileX(heroHeadingX);
    int heroHeadingTileY = tileset_->convertKnightY2TileY(heroHeadingY);

    if(nextX_ != heroHeadingTileX && nextY_ != heroHeadingTileY) {
        // we need to turn
        knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_UP);
    } else if(nextX_ == heroHeadingTileX && nextY_ == heroHeadingTileY) {
        // What is next? Should the hero keep walking or not?
        --stepIndex_;
        if(stepIndex_ >= 0 && findNextTileForHero()) {
            // do we need to turn?
            if((abs(knight_->directionX()) > .5 && nextY_ != heroHeadingTileY) || (abs(knight_->directionY()) > .5 && nextX_ != heroHeadingTileX)) {
                // we need to turn
                knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_UP);
                ++stepIndex_;
            }
        } else {
            return true;
        }
    }


    if(knight_->isReady()) {
        waitForWalk_ = false;
        --stepIndex_;
        if(stepIndex_ < 0) {
            quit();
            return true;
        }
    } else {
        return true;
    }
    return false;
}

void HeroWalkStrategy::finishStrategy() noexcept {
    stepIndex_ = -1;
}

void HeroWalkStrategy::goToTheNextTile() {
    if(stepIndex_ == -1) return;
    // go to the next tile
    if(findNextTileForHero()) {
        if(heroTileX_ == targetX_ && heroTileY_ == targetY_) {
           stepIndex_ = -1;
        } else if(heroTileX_ > (int)nextX_) {
            btnLast_ = &btnLeft_;
            knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
            waitForWalk_ = true;
        } else if(heroTileX_ < (int)nextX_) {
            btnLast_ = &btnRight_;
            knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
            waitForWalk_ = true;
        } else if(heroTileY_ > (int)nextY_) {
            btnLast_ = &btnUp_;
            knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
            waitForWalk_ = true;
        } else if(heroTileY_ < (int)nextY_) {
            btnLast_ = &btnDown_;
            knight_->onButtonEvent(btnLast_, ButtonEventListener::ButtonState::BS_DOWN);
            waitForWalk_ = true;
        }
    }
}

void HeroWalkStrategy::followThePath() {
    if(stepIndex_ < 0) {
        quit();
        return;
    }
    if(waitForWalk_) {
        if(waitForHeroWalk()) return;
    }
    goToTheNextTile();
}

void HeroWalkStrategy::doActions(Knight* knight) noexcept {
    knight_ = knight;
    tileset_ = Application::tileset();

    // current tile position of the hero
    heroTileX_ = tileset_->convertKnightX2TileX(knight->x());
    heroTileY_ = tileset_->convertKnightY2TileY(knight->y() + 2);

    // Let's convert the content of the tileset for A* algorithm,
    // and follow the path
    if(!aStarResult_.size()) {
        findPath();
    } else {
        followThePath();
    }
}

Strategy<Knight>* HeroWalkStrategy::copy() const noexcept {
    return new HeroWalkStrategy(*this);
}
