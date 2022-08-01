#include "Environment.h"
#include "MediaData.h"
#include <algorithm>
#include "Global.h"
#include "World.h"
#include "RPGBgElement.h"
#include "ObserverSubjectVictoryEventVisitor.h"
#include "Application.h"
#include <functional>
#include "HeroStraightWalkStrategy.h"
#include "BitmapIds.h"
#include "SoundEffect.h"

//using namespace std::placeholders;

void Environment::init(WorldUPtr& world, TilesetUPtr&& tileset) noexcept {
    Application::setPlayMode(Application::PlayMode::REALTIME); // default
    gravity_ = std::make_unique<StaticObjects>();
    gameMap_ = std::make_unique<GameMap>();
    gameMap_->addBackgroundBitmapId(NATIVE_BITMAP_BACKGROUND);

    tileset_ = std::move(tileset);
    tileset_->init(world);

    //std::cout << "Tileset position: " << tileset_->position().x << ", " << tileset_->position().y << "\n";
    //std::cout << "Tileset size: " << tileset_->width() * tileset_->tileWidth() << ", " << tileset_->height() * tileset_->tileHeight() << "\n";

    gameMap_->setSize(std::max((uint32_t)abs(tileset_->position().x) + tileset_->widthInPixels(), screenWidth()),
                      std::max((uint32_t)abs(tileset_->position().y) + tileset_->heightInPixels(), screenHeight()));
    //std::cout << "Environment::init, gameMap: " << gameMap_->size() << "\n";
    //std::cout << "Screen width: " << screenWidth() << ", height: " << screenHeight() << "\n";
#ifndef __SERVERAPP__
    gameMap_->cacheBackground();
#endif
}

bool Environment::removeHole(RollingStone* stone) noexcept {
    auto& world = Application::latestWorld();
    Rect rect2 = stone->collidedRect();
    for(auto& possibleHole : world->rpgBgElementList()) {
        if(!possibleHole.second->collisionData()) continue;

        Rect rect1 = possibleHole.second->collisionData()->rect();
        rect1.x += possibleHole.second->x();
        rect1.y += possibleHole.second->y();

        if(!(rect1 == rect2)) continue;

        holeX_ = possibleHole.second->x();
        holeY_ = possibleHole.second->y();
        //std::cout << "Hole rect: " << rect1 << "; " << rect2 << "\n";
        // The hole and the stone must be removed from the tileset
        possibleHole.second->die();
        tileset_->removeItem(possibleHole.second->id());
        tileset_->removeItem(stone->id());
        if(!world->multiplayer()) {
            SoundEffect::play(SoundEffect::Name::PLAY_GOAL);
        }
        return true;
    }
    return false;
}

void Environment::finishGoalKick(const KnightUPtr& k) noexcept {
    // The knight should stop after the stone is pushed into the hole
    // as long as we are in turn-based and multiplayer mode
    if(k->strategy()) return;
    activeKnightId_ = Environment::NONE_ACTIVE_KNIGHT; // no more button event
    int holeTileX = tileset_->convertWindowX2TileX(holeX_);
    int holeTileY = tileset_->convertWindowY2TileY(holeY_);
    auto hs = new HeroStraightWalkStrategy(tileset_->convertTileX2KnightX(holeTileX), tileset_->convertTileY2KnightY(holeTileY));
    hs->addOption(HeroStraightWalkStrategy::Option::SIT_DOWN_AT_THE_END);
    hs->addOption(HeroStraightWalkStrategy::Option::ACTIVATE_NEXT_TURN);
    k->setStrategy(hs);
}

void Environment::notify(ObserverSubjectVictoryEvent* subject) {
    class V : public ObserverSubjectVictoryEventVisitor {
    public:
        void visit(RollingStone* p) noexcept override {
            p_ = p;
        }
        RollingStone* p_{};
    };
    V stone;
    subject->accept(&stone);
    if(!stone.p_) return;

    // the hole must be removed from the rest of the stone's list
    if(!removeHole(stone.p_)) return;
    // stone should disappear
    stone.p_->die();
    // score of the active hero should be increased
    auto& world = Application::latestWorld();
    auto it = world->knightList().find(lastStoodUpKnightId_);
    if(it != world->knightList().end()) {
        auto& k = it->second;
        incScore(k->id());

        if(Application::playMode() == Application::PlayMode::TURNBASED
        && Application::playerMode() == Application::PlayerMode::MULTIPLAYER) {
            finishGoalKick(k);
        }
    }
}

// TODO: this should work with team ids
void Environment::incScore(int id) noexcept {
    auto it = score_.find(id);
    if(it == score_.end()) {
        score_[id] = 1;
    } else {
        ++it->second;
    }
}

int Environment::score(int id) const noexcept {
    auto it = score_.find(id);
    return it == score_.end() ? 0 : it->second;
}

void Environment::setActiveKnightId(uint32_t id) noexcept {
    activeKnightId_ = id;
    if(activeKnightId_ == Environment::NONE_ACTIVE_KNIGHT) {
        // the next turn button label must be changed
        Application::changeNextTurnButtonLabel(TranslateInterface::Key::TRA__LABEL_PASS);
    } else {
        // the next turn button label must be changed
        Application::changeNextTurnButtonLabel(TranslateInterface::Key::TRA__LABEL_NEXT_TURN);
    }
}
