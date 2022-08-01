#include "SingleplayerGame.h"
#include "World.h"
#include "Button.h"
#include "WorldTouchEvent.h"
#include "World.h"
#include "Application.h"

SingleplayerGame::SingleplayerGame() {

}

void SingleplayerGame::updateWorld([[maybe_unused]]double now) noexcept {
    std::for_each(buttons().begin(), buttons().end(), [now](Button*b){b->update(now);});

    if(latestWorld()->status() == World::Status::ST_DEFEAT || latestWorld()->status() == World::Status::ST_VICTORY) {
        Application::nextLevel();
        return;
    } else {
        updateANewCopyOfTheLastestWorld(now);
    }
    if(worldList().size() > WORLDLIST_MAX_SIZE) {
        worldList().erase(worldList().begin());
    }
}

void SingleplayerGame::drawWorld([[maybe_unused]]AndroidBitmapInfo* info, [[maybe_unused]]void* pixels) noexcept {
    WorldUPtr& world = latestWorld();
    world->deleteDeadObjects();
    world->changeState();
    world->draw(info, pixels);
    std::for_each(buttons().begin(), buttons().end(), [&info, &pixels](Button*b){b->draw(info, pixels);});
}
