#include "GameObjectCollider.h"
#include "MultiplayerWorld.h"
#include "Application.h"
#include "Rect.h"
#include "MediaData.h"
#include "Vector.h"
#include "Math_.h"
#include "Environment.h"
#include "StaticObjects.h"
#include "Algorithms.h"
#include "BitmapIds.h"
#include "HeroStraightWalkStrategy.h"
#include "CardPushPlayer.h"
#include "TilesetCardHandler.h"

using namespace Algorithms;

// special collisions
// ------------------
// knight - stone (ball)
// stone  - stone
// knight - Islands
// knight - rpg elements
// stone  - rpg elements
// ---------------------
// knight - knight ??
// stone  - knight ??

/*
 There are other checks in order to prevent the collisions. See StateKnightRun::isNextTileValid, Tileset::isBlockingTile
 */

bool GameObjectCollider::testHeroStoneCollisions(KnightUPtr& hero) noexcept {
    if(!hero->collisionData()) return false;

    auto tch = Application::tileset()->tilesetCardHandler();
    if(tch && tch->getActivePushPlayerCard() && testHeroHeroCollisions(hero)) {

        if(collidedObject_ && collidedObject_->knight()) {
            auto knight = collidedObject_->knight();
            Tileset* tileset = Application::tileset();

            // we have to check the tile behind the collided hero
            int nextTileXCollided = tileset->convertKnightX2TileX(knight->x());
            int nextTileYCollided = tileset->convertKnightY2TileY(knight->y());

            if(hero->directionX() > .5) ++nextTileXCollided;
            if(hero->directionY() > .5) ++nextTileYCollided;
            if(hero->directionX() < -.5) --nextTileXCollided;
            if(hero->directionY() < -.5) --nextTileYCollided;


            if(tileset->isBlockingTile(nextTileXCollided, nextTileYCollided, hero->direction())) {
                return true;
            }

            for(auto& thirdKnight : world_->knightList()) {
                int thirdKnightX = tileset->convertKnightX2TileX(thirdKnight.second->x());
                int thirdKnightY = tileset->convertKnightY2TileY(thirdKnight.second->y());
                if(thirdKnightX == nextTileXCollided && thirdKnightY == nextTileYCollided) {
                    debug("GameObjectCollider::testHeroStoneCollisions true");
                    return true;
                }
            }

            if(knight->push(hero->directionX(), hero->directionY(), hero->velocity() * 2.1)) {

                Application::environment()->setActiveKnightId(std::numeric_limits<uint32_t>::max()); // no more button event
                int nextTileX = tileset->convertKnightX2TileX(hero->x());
                int nextTileY = tileset->convertKnightY2TileY(hero->y());

                if(hero->directionX() > .5) ++nextTileX;
                if(hero->directionY() > .5) ++nextTileY;


                auto hs = new HeroStraightWalkStrategy(tileset->convertTileX2KnightX(nextTileX), tileset->convertTileY2KnightY(nextTileY));
                if(world_->multiplayer() || Application::activeMenu() == MenuInterface::MenuType::MULTIPLAYER_ON_ONE_DEVICE_RUNNING) {
                    hs->addOption(HeroStraightWalkStrategy::Option::SIT_DOWN_AT_THE_END);
                }
                hs->addOption(HeroStraightWalkStrategy::Option::ACTIVATE_NEXT_TURN);
                hero->setStrategy(hs);

                auto activePushPlayerCard = tch->getActivePushPlayerCard();
                activePushPlayerCard->resetCoolDown();
                if(Application::activeMenu() != MenuInterface::MenuType::SINGLEPLAYER_RUNNIG) {
                    activePushPlayerCard->setActive(false);
                }
                tileset->incStonePushCounter();
                return false;
            }
        }
    }


    int heroX = hero->x();
    int heroY = hero->y();
    const Rect heroRect = hero->collisionData()->rect();
    for( auto it = world_->rollingStoneList().begin(); it != world_->rollingStoneList().end(); ++it) {
        if(it->second->isDead() || !it->second->collisionData() || it->second->collisionData()->rect().isNull()) continue;
        int rollingStoneX = it->second->x();
        int rollingStoneY = it->second->y();

        const Rect elementRect = it->second->collisionData()->rect();
        while(Collision::detectRectCollision(heroX, heroY, heroRect, rollingStoneX, rollingStoneY, elementRect, 0, 0))
        {
            //std::cout << heroX << ", " << heroY << ", " << heroRect << ", " << rollingStoneX << ", " << rollingStoneY << ", " << elementRect << "\n";

            hero->handleCollision(it->second.get());
            // hero connot push a stone with another stone, and cannot push a stone through an RPGBgElement
            if(testStoneCollisions(it->second) || testOneManyGameObjectCollision(it->second.get(), world_->rpgBgElementList())) {
                // revert the stone's move
                //it->second->push(-directionX, -directionY);
                it->second->setX(rollingStoneX);
                it->second->setY(rollingStoneY);
                return true;
            }
            rollingStoneX = it->second->x();
            rollingStoneY = it->second->y();
        }
    }
    return false;
}

bool GameObjectCollider::testHeroHeroCollisions(KnightUPtr& hero) noexcept {
    return testOneManyGameObjectCollision(hero.get(), world_->knightList());
}

// Collision test with Islands and with other stones
bool GameObjectCollider::testStoneCollisions(const RollingStoneUPtr& stone) noexcept {
    constexpr int thresholdX1 = -5;
    //constexpr int thresholdX2 = -1;
    constexpr int thresholdY  = 0;
    //constexpr int precentageY = -50;

    // Collision test with Islands
    if(Application::environment()->gravity()->detectRectCollision(stone.get(), thresholdY, thresholdX1, 0)) {
        return true;
    }
    return testOneManyGameObjectCollision(stone.get(), world_->rollingStoneList());
}

bool GameObjectCollider::testHeroRPGBgElementCollisions(const KnightUPtr& hero) noexcept {
    return testOneManyGameObjectCollision(hero.get(), world_->rpgBgElementList());
}

// Exceptions
bool GameObjectCollider::isCollisionAlloved(GameObject* g1, GameObject* g2) const noexcept {
    // There is no collision between lake and rolling stone ball
    if((g1->skinMin() == NATIVE_BITMAP_LAKE_01 && anyOf(g2->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))
    || (g2->skinMin() == NATIVE_BITMAP_LAKE_01 && anyOf(g1->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))) {
        return false;
    }
    if((g1->skinMin() == NATIVE_BITMAP_HOLE_01 && anyOf(g2->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))
    || (g2->skinMin() == NATIVE_BITMAP_HOLE_01 && anyOf(g1->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))) {
        return false;
    }

    if((g1->skinMin() == NATIVE_BITMAP_HOLE_RED_01 && anyOf(g2->skinMin(), NATIVE_BITMAP_STONEBALL_RED_01, NATIVE_BITMAP_STONEBALL_RED_02))
    || (g2->skinMin() == NATIVE_BITMAP_HOLE_RED_01 && anyOf(g1->skinMin(), NATIVE_BITMAP_STONEBALL_RED_01, NATIVE_BITMAP_STONEBALL_RED_02))) {
        return false;
    }
    if((g1->skinMin() == NATIVE_BITMAP_HOLE_BLUE_01 && anyOf(g2->skinMin(), NATIVE_BITMAP_STONEBALL_BLUE_01, NATIVE_BITMAP_STONEBALL_BLUE_02))
    || (g2->skinMin() == NATIVE_BITMAP_HOLE_BLUE_01 && anyOf(g1->skinMin(), NATIVE_BITMAP_STONEBALL_BLUE_01, NATIVE_BITMAP_STONEBALL_BLUE_02))) {
        return false;
    }
    if((g1->skinMin() == NATIVE_BITMAP_HOLE_DARK_01 && anyOf(g2->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))
    || (g2->skinMin() == NATIVE_BITMAP_HOLE_DARK_01 && anyOf(g1->skinMin(), NATIVE_BITMAP_STONEBALL_01, NATIVE_BITMAP_STONEBALL_02))) {
        return false;
    }
    return true;
}
