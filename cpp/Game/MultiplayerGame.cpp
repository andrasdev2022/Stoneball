#include "MultiplayerGame.h"
#include "World.h"
#include "Button.h"
#include "WorldTouchEvent.h"
#include "Application.h"
#include "ClientTimeSyncer.h"
#include "MultiplayerWorld.h"

MultiplayerGame::MultiplayerGame() {

}

void MultiplayerGame::updateWorld([[maybe_unused]]double now) noexcept {
    std::for_each(buttons().begin(), buttons().end(), [now](Button*b){b->update(now);});
    emotesBubble_->update(now);
    // Multiplayer client does not prepare a copy from the world
    latestWorld()->update(now);
    if(worldList().size() > WORLDLIST_MAX_SIZE) {
        worldList().erase(worldList().begin());
    }
}

void MultiplayerGame::drawWorld([[maybe_unused]]AndroidBitmapInfo* info, [[maybe_unused]]void* pixels) noexcept {
    WorldUPtr& world = latestWorld();
    world->deleteDeadObjects();
    world->changeState();

    WorldUPtr interpolatedWorld = entityInterpolation(Application::now());
    interpolatedWorld->draw(info, pixels);
    std::for_each(buttons().begin(), buttons().end(), [&info, &pixels](Button*b){b->draw(info, pixels);});
    emotesBubble_->draw(info, pixels);
}

WorldUPtr MultiplayerGame::entityInterpolation(double now) noexcept {
    // Let's find the world
    const double interpolationTimeStamp = now - ClientTimeSyncer::interpolationTime_;
    auto it = worldList().rbegin();
    while(it != worldList().rend()) {
        if(it->second->multiplayer()->snapshotTimeStamp() >= interpolationTimeStamp) {
            ++it;
            //debug("DBG entityInterpolation step back");
        } else {
            break;
        }
    }

    if(it == worldList().rend()) {
        it = worldList().rbegin();
        //debug("DBG entityInterpolation it set to rbegin");
    }
    auto itNext = std::next(it, 0);
    if(itNext != worldList().rbegin()) {
        itNext = std::prev(it, 1);
        //debug("DBG entityInterpolation itNext set to it + 1");
    }
    WorldUPtr world = std::unique_ptr<World>(it->second->copy());

    // Let's do the interpolation
    if(world->multiplayer() && itNext->second->multiplayer()) {
        world->multiplayer()->interpolate(itNext->second->multiplayer(), interpolationTimeStamp);
    }
    return world;
}
