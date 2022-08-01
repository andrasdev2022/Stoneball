#include "MultiplayerGameServerSide.h"
#include "World.h"
#include "Button.h"
#include "WorldTouchEvent.h"

MultiplayerGameServerSide::MultiplayerGameServerSide() {

}

void MultiplayerGameServerSide::updateWorld([[maybe_unused]]double now) noexcept {
    std::for_each(buttons().begin(), buttons().end(), [now](Button*b){b->update(now);});
    updateANewCopyOfTheLastestWorld(now);
    if(worldList().size() > WORLDLIST_MAX_SIZE) {
        worldList().erase(worldList().begin());
    }
}

void MultiplayerGameServerSide::drawWorld([[maybe_unused]]AndroidBitmapInfo* info, [[maybe_unused]]void* pixels) noexcept {
    WorldUPtr& world = latestWorld();
    world->deleteDeadObjects();
    world->changeState();
}
