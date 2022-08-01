#ifndef __MULTIPLAYERWORLD_H__
#define __MULTIPLAYERWORLD_H__

#include "TouchEventListener.h"
#include "Knight.h"
#include "Particle.h"
#include "android.h"
#include "World.h"
#include "UDPServerSnapshot.h"
#include "UDPServerDeltaSnapshot.h"
#include "GameObjectCollider.h"

#include <map>
#include <algorithm>

class UDPServerSnapshot;
class MultiplayerWorld;
using MultiplayerWorldUPtr = std::unique_ptr<MultiplayerWorld>;
enum BitmapId : int;

class MultiplayerWorld /*final*/ : public World, public TouchEventListener {
public:
    friend class SnapshotSyncer;
    friend class GameObjectCollider;
    MultiplayerWorld() noexcept = default;
    void init() noexcept override;
    MultiplayerWorld(const MultiplayerWorld& w) noexcept;
    MultiplayerWorld(MultiplayerWorld&& w) = delete;
    void operator=(MultiplayerWorld&& w) = delete;

    // World
    void changeState() noexcept override;
    Status status() const noexcept override {return status_;}
    void update(double now) noexcept override;
    void draw(AndroidBitmapInfo* info, void* pixels) noexcept override;
    void draw2DRPG() noexcept;
    void deleteDeadObjects() noexcept override;
    bool isHeroDead() const noexcept override {return status_ == Status::ST_HERO_IS_DEAD;}
    bool areEnemiesDead() const noexcept override {return status_ == Status::ST_ENEMIES_ARE_DEAD;}
    void setDefeat() noexcept override {status_ = Status::ST_DEFEAT;}
    double timeOfDeath() const noexcept override {return timeOfDeath_;}
    void wakeUp(double now) noexcept override;
    World* copy() const noexcept override;
    MultiplayerWorld* multiplayer() noexcept override {return this;}

    virtual bool gameOver() const noexcept {return status_ == Status::ST_DEFEAT || status_ == Status::ST_VICTORY;}

    // ECS
    //virtual void initEntity(const UDPMessageData& msgData) noexcept;
    //virtual void updateEntity(EntityInterfaceUPtr& entity, const UDPMessageData& msgData) noexcept;

    virtual void sync(UDPServerSnapshot*) noexcept(false);
    virtual void sync(UDPServerDeltaSnapshot*) noexcept(false);
    virtual bool isAnyoneDead() noexcept;

    // Every player receives his/her client id from the server
    virtual void setClientId(uint32_t clientId) noexcept {clientId_ = clientId;}
    virtual uint32_t clientId() const noexcept {return clientId_;}

    virtual void interpolate(MultiplayerWorld* world, double interpolatedTime) noexcept;
    virtual uint32_t tick() const noexcept {return tick_;}
    virtual double snapshotTimeStamp() const noexcept {return snapshotTimeStamp_;}

    // TouchEventListener
    bool onTouchEvent(int action, double x, double y) noexcept override;


    // ButtonEventListener
    bool onButtonEvent(GameObject* sender, ButtonState buttonState) noexcept override;
    virtual bool onButtonEventServer(uint32_t clientId, GameObject* sender, ButtonState buttonState) noexcept;

    void onActivateKnight(uint32_t activeKnightId) noexcept;
    void onActivateKnightServer(uint32_t clientId, uint32_t activeKnightId) noexcept;
    void onTileClicked(int tileX, int tileY) noexcept;
    void onTileClickedServer(uint32_t clientId, int tileX, int tileY) noexcept;
    void onDeckClicked(int deckIndex) noexcept;
    void onDeckClickedServer(uint32_t clientId, int deckIndex) noexcept;
    void onCardClicked(uint32_t id) noexcept;
    void onCardClickedServer(uint32_t clientId, uint32_t  cardId) noexcept;
    void onHideAllCards() noexcept;
    void onHideAllCardsServer(uint32_t clientId) noexcept;
    void onBallToPullClicked(uint32_t ballId);
    void onBallToPullClickedServer(uint32_t clientId, uint32_t ballId);
    void onEmoteBubbleSelected(uint32_t emoteId) noexcept;
    void onEmoteBubbleSelectedServer(uint32_t clientId, uint32_t emoteId, uint8_t avatar) noexcept;

protected:
    void drawBitmap(BitmapId id, AndroidBitmapInfo* info, void* pixels) noexcept;
    template<typename T> bool isAnyoneDead2(T&) noexcept;
    void sendKeepAlive() noexcept;
    void opponentCheck() noexcept;
    void interpolateScreen() noexcept;

    Status status_;
    double timeOfDeath_{0.};
    uint32_t clientId_{0};
    uint32_t tick_{};
    double snapshotTimeStamp_{};
    double keepAlive_{};
    uint32_t waitingForOpponentsMeesageboxId_{};
};

template<typename T>
bool MultiplayerWorld::isAnyoneDead2(T& objects) noexcept {
    for(auto& obj : objects) {
        if(obj.second->isDead()) {
            return true;
        }
    }
    return false;
}


#endif // __MULTIPLAYERWORLD_H__
