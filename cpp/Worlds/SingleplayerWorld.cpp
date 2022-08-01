#include "SingleplayerWorld.h"
#include "stoneball.h"
#include "Knight.h"
#include "TouchEventHandler.h"
#include "Button.h"
#include "Collision.h"
#include "StaticObjects.h"
#include "MediaData.h"
#include "Vector.h"
#include "Screen.h"
#include "GameMap.h"
#include "Bitmap565.h"
#include "WorldTouchEvent.h"
#include "Application.h"
#include <algorithm>
#include "Environment.h"
#include "GameObjectCollider.h"
#include "BitmapIds.h"
//#include "DrawEntityInterface.h"
#include "SoundEffect.h"
#include "TilesetCardHandler.h"

using namespace Bitmap565;
using namespace Application;

#include <stdlib.h>
#include <list>

void SingleplayerWorld::init() noexcept
{
    srand(time(0));
    status_ = Status::ST_ONGOING_GAME;
    if(Application::playMode() != Application::PlayMode::TURNBASED) {
        for(auto& knight : knights_) {
            if(knight.second->type() == GameObjectInterface::Type::GOT_KNIGHT_RED) {
                knight.second->getUp();
                Application::environment()->setActiveKnightId(knight.second->id());
            }
        }
    }
}

SingleplayerWorld::SingleplayerWorld(const SingleplayerWorld& w) noexcept
 : status_(w.status_)
 , timeOfDeath_(w.timeOfDeath_)
 , waitForResultPanel_(w.waitForResultPanel_)
 , redrawLevelCompletedPanel_(w.redrawLevelCompletedPanel_)
 {
    copyEntities(w.knights_, knights_);
    copyEntities(w.particles_, particles_);
    copyEntities(w.rollingStones_, rollingStones_);
    copyEntities(w.rpgBgElements_, rpgBgElements_);
    copyEntitiesV2(w.otherElements_, otherElements_);
    //copyEntitiesV2(w.entities_, entities_);
}

World* SingleplayerWorld::copy() const noexcept {
    return new SingleplayerWorld(*this);
}
/*
namespace {
void playKnightSoundEffect(GameObject* knight, State::Type stateType) noexcept {
    Unused(0, stateType, knight);
    switch(stateType) {
    case State::Type::ST_RUN:
        SoundEffect::play(SoundEffect::Name::PLAY_RUN_ON_SAND);
        break;
    default:
        //SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
        break;
    }
}
}
*/
void SingleplayerWorld::update(double now) noexcept {
    bool someoneIsRunning = false;
    std::for_each(knights_.begin(), knights_.end(), [&](std::pair<const uint32_t, KnightUPtr>& p){
        double posX = p.second->x();
        double posY = p.second->y();
        p.second->update(now);
        GameObjectCollider collider(this);
        if(collider.testHeroStoneCollisions(p.second) || collider.testHeroRPGBgElementCollisions(p.second)) {
            debug("SingleplayerWorld::update: Hero is reverted");
            // hero's move must be reverted
            p.second->setX(posX);
            p.second->setY(posY);
            return;
        }
        someoneIsRunning |= (p.second->state()->stateType() == State::Type::ST_RUN);
        //playKnightSoundEffect(p.second.get(), p.second->state()->stateType());
    });

    if(someoneIsRunning) {
        SoundEffect::play(SoundEffect::Name::PLAY_RUN_ON_SAND);
    } else if(SoundEffect::isEmpty()) {
        SoundEffect::play(SoundEffect::Name::STOP_PLAYING_RUN_ON_SAND);
    }

    updateEntities(particles_, now);
    updateEntities(rollingStones_, now);
    updateEntities(rpgBgElements_, now);
    updateEntities(otherElements_, now);
    //updateEntities2(entities_, now);
    Application::tileset()->followTheKnight();
    setScreenPos(screen()->x(), screen()->y());
    Application::tileset()->update(now);
    if(Application::tileset()->tilesetCardHandler()) {
        Application::tileset()->tilesetCardHandler()->updateDecks(Application::now());
    }
    //LOGI("DBG SingleplayerWorld::update end");
}

void SingleplayerWorld::changeState() noexcept {
    changeEntitiesState(knights_);
    //changeEntitiesState2(entities_);
}

void SingleplayerWorld::drawLevelCompletedPanel(AndroidBitmapInfo* info, void* pixels) noexcept {
    if(Application::language() == TranslateInterface::Language::ENGLISH) {
        drawBitmap(NATIVE_BITMAP_LEVEL_COMPLETED_ENG, info, pixels);
    } else if(Application::language() == TranslateInterface::Language::HUNGARIAN) {
        drawBitmap(NATIVE_BITMAP_LEVEL_COMPLETED_HUN, info, pixels);
    } else if(Application::language() == TranslateInterface::Language::BULGARIAN) {
        drawBitmap(NATIVE_BITMAP_LEVEL_COMPLETED_BUL, info, pixels);
    }
}

void SingleplayerWorld::draw(AndroidBitmapInfo* info, void* pixels) noexcept {
    //LOGI("DBG SingleplayerWorld::draw begin");
    Unused(0, info);

    screen()->setContentPtr((uint16_t*)pixels);

    if(rollingStones_.size() == 0 && ++waitForResultPanel_ > 10) {
        if(status_ == Status::ST_ENEMIES_ARE_DEAD || status_ == Status::ST_VICTORY) {
            if(redrawLevelCompletedPanel_) {
                drawLevelCompletedPanel(info, pixels);
                redrawLevelCompletedPanel_ = false;
            }
            return;
        }
        //std::cout << "waitForResultPanel_!!!!!!!!!!!!!!!!!" << waitForResultPanel_ << "\n";
        timeOfDeath_ = Application::now();
        status_ = Status::ST_ENEMIES_ARE_DEAD;
        drawLevelCompletedPanel(info, pixels);

        // Java will save the status in the settings file
        Application::saveLevelCompleted((int)Application::gameLevel());
        return;
    }

    screen()->copyPreparedBackground(environment()->gameMap());

    draw2DRPG();
}

void SingleplayerWorld::draw2DRPG() noexcept {
    struct Less {
        bool operator()(GameObject* a, GameObject* b) const {
            return a->z() < b->z();
        }
    };
    std::multiset<GameObject*, Less> sorter;
    std::for_each(knights_.begin(), knights_.end(), [&sorter](const auto& knightData){sorter.insert(knightData.second.get());});
    std::for_each(rpgBgElements_.begin(), rpgBgElements_.end(), [&sorter](const auto& rpgBgElementData){sorter.insert(rpgBgElementData.second.get());});
    std::for_each(rollingStones_.begin(), rollingStones_.end(), [&sorter](const auto& rollingStoneData){sorter.insert(rollingStoneData.second.get());});
    std::for_each(otherElements_.begin(), otherElements_.end(), [&sorter](const auto& blueTriangleData){sorter.insert(blueTriangleData.second.get());});
    std::for_each(sorter.begin(), sorter.end(), [](const auto& p){p->draw(screen());});
    //for_each(entities_.begin(), entities_.end(), [](auto& entityPair){
    //    DrawEntityInterfaceNS::draw(entityPair.second, screen());
    //});
    Application::tileset()->drawTexts();
}

void SingleplayerWorld::drawBitmap(BitmapId id, AndroidBitmapInfo* info, void* pixels) noexcept {
    MediaData* data = bitmapData(id, false);
    int gameOverWidth = bitmapWidth(id);
    int gameOverHeight = bitmapHeight(id);
    Rect rect(0, 0, gameOverWidth, gameOverHeight);
    uint16_t* dst = (uint16_t*)pixels + info->width * ((info->height - gameOverHeight) / 2)  + (info->width - gameOverWidth) / 2;

    copyIntersectionRarematrix(dst, rect, info->width, data->data(), rect);
}

bool SingleplayerWorld::onTouchEvent(int action, double x, double y) noexcept {
    Unused(0, action, x, y);
    return false;
}

bool SingleplayerWorld::onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept {
    if(knights_.size()) {
        for(auto& knightData : knights_){
            if(knightData.second->id() == Application::environment()->activeKnightId()) {
                if(knightData.second->onButtonEvent(sender, buttonState)) {
                    return true;
                }
            }
        }
    }
    return false;
}

void SingleplayerWorld::deleteDeadObjects() noexcept {
    deleteDeadObjects2(particles_);
    deleteDeadObjects2(rollingStones_);
    deleteDeadObjects2(rpgBgElements_);
    deleteDeadObjects2(otherElements_);
    //deleteDeadObjects3(); // for entities
}

void SingleplayerWorld::wakeUp(double now) noexcept {
    wakeUpEntities(knights_, now);
    wakeUpEntities(particles_, now);
    wakeUpEntities(rollingStones_, now);
    wakeUpEntities(rpgBgElements_, now);
    wakeUpEntities(otherElements_, now);
    //wakeUpEntities2(now);

    if(timeOfDeath_ > 1e-7) {
        timeOfDeath_ = now;
    }
}
