#ifndef __WORLD_H__
#define __WORLD_H__

#include <memory>
#include "ButtonEventListener.h"
#include "Knight.h"
#include "Particle.h"
#include "RollingStone.h"
#include "RPGBgElement.h"
#include "BlueTriangle.h"
#include <map>
#include <algorithm>
//#include "EntityInterface.h"
//#include "UpdateEntityInterface.h"
//#include "ChangeEntityStateInterface.h"
//#include "EntityHelper.h"

class SingleplayerWorld;
class MultiplayerWorld;

class World : public ButtonEventListener {
public:
    enum class Status {
        ST_ONGOING_GAME,
        ST_HERO_IS_DEAD,
        ST_ENEMIES_ARE_DEAD,
        ST_DEFEAT,
        ST_VICTORY
    };
    virtual ~World() = default;
    virtual void init() noexcept = 0; // I should call the init functions from this function instead of constructor
    virtual void update(double now) = 0;
    virtual void draw(AndroidBitmapInfo* info, void* pixels) = 0;
    virtual void deleteDeadObjects() = 0;

    virtual void changeState() = 0;
    virtual bool isHeroDead() const = 0;
    virtual bool areEnemiesDead() const = 0;
    virtual double timeOfDeath() const = 0;
    virtual void setDefeat() = 0;
    virtual Status status() const = 0;
    virtual void wakeUp(double now) = 0;
    virtual World* copy() const = 0;
    virtual SingleplayerWorld* singleplayer() noexcept {return nullptr;}
    virtual MultiplayerWorld* multiplayer() noexcept {return nullptr;}

    virtual GameObject* findGameObject(uint32_t gameObjectId) noexcept {
        auto g = findGameObject(knights_, gameObjectId);
        if(!g) {g = findGameObject(particles_, gameObjectId);}
        if(!g) {g = findGameObject(rollingStones_, gameObjectId);}
        if(!g) {g = findGameObject(rpgBgElements_, gameObjectId);}
        if(!g) {g = findGameObject(otherElements_, gameObjectId);}
        return g;
    }

    virtual const std::map<uint32_t, KnightUPtr>& knightList() const noexcept {return knights_;}
    virtual const std::map<uint32_t, ParticleUPtr>& particleList() const noexcept {return particles_;}
    virtual const std::map<uint32_t, RollingStoneUPtr>& rollingStoneList() const noexcept {return rollingStones_;}
    virtual const std::map<uint32_t, RPGBgElementUPtr>& rpgBgElementList() const noexcept {return rpgBgElements_;}
    virtual const std::map<uint32_t, BlueTriangleUPtr>& otherElementList() const noexcept {return otherElements_;}
    //virtual const std::map<uint32_t, EntityInterfaceUPtr>& entityList() const noexcept {return entities_;}

    virtual void addKnight(KnightUPtr&& knight) {knights_.emplace(knight->id(), std::move(knight));}
    virtual void addParticle(ParticleUPtr&& particle) {particles_.emplace(particle->id(), std::move(particle));}
    virtual void addRPGBgElement(RPGBgElementUPtr&& element) {rpgBgElements_.emplace(element->id(), std::move(element));}
    virtual void addRollingStone(RollingStoneUPtr&& stone) {rollingStones_.emplace(stone->id(), std::move(stone));}
    virtual void addOtherElement(BlueTriangleUPtr&& gameObject) {otherElements_.emplace(gameObject->id(), std::move(gameObject));}
//    virtual void addEntity(EntityInterfaceUPtr&& entity) {entities_.emplace(entity->id(), std::move(entity));}

protected:

    template <typename S>
    void copyEntities(const std::map<uint32_t, std::unique_ptr<S>>& src, std::map<uint32_t, std::unique_ptr<S>>& target) {
        std::for_each(src.begin(), src.end(),
            [&target](const auto& p){
                target.emplace(p.first, new S(*p.second.get()));
            }
        );
    }

    template <typename S>
    void copyEntitiesV2(const std::map<uint32_t, std::unique_ptr<S>>& src, std::map<uint32_t, std::unique_ptr<S>>& target) {
        std::for_each(src.begin(), src.end(),
            [&target](const auto& p){
                target.emplace(p.first, p.second->copy());
            }
        );
    }

    template <typename S>
    void updateEntities(const std::map<uint32_t, std::unique_ptr<S>>& map, double now) {
        std::for_each(map.begin(), map.end(),
            [now](const auto& p){
                p.second->update(now);
            }
        );
    }
/*
    template <typename S>
    void updateEntities2(const std::map<uint32_t, std::unique_ptr<S>>& map, double now) {
        std::for_each(map.begin(), map.end(),
            [now](const auto& p){
                UpdateEntityInterfaceNS::update(p.second.get(), now);
            }
        );
    }
*/
    template <typename S>
    void changeEntitiesState(const std::map<uint32_t, std::unique_ptr<S>>& map) {
        std::for_each(map.begin(), map.end(),
            [](const auto& p){
                p.second->changeState();
            }
        );
    }
/*
    template <typename S>
    void changeEntitiesState2(const std::map<uint32_t, std::unique_ptr<S>>& map) {
        std::for_each(map.begin(), map.end(),
            [](const auto& p){
                ChangeEntityStateInterfaceNS::changeState(p.second.get());
            }
        );
    }
*/
    template <typename S>
    void drawEntities(const std::map<uint32_t, std::unique_ptr<S>>& map, Screen* screen) {
        std::for_each(map.begin(), map.end(),
            [screen](const auto& p){
                p.second->GameObject::draw(screen);
            }
        );
    }

    template <typename S>
    void wakeUpEntities(const std::map<uint32_t, std::unique_ptr<S>>& map, double now) {
        std::for_each(map.begin(), map.end(),
            [now](const auto& p){
                p.second->wakeUp(now);
            }
        );
    }
/*
    void wakeUpEntities2(double now) {
        std::for_each(entities_.begin(), entities_.end(),
            [now](const auto& p){
                EntityHelper::wakeUp(p.second.get(), now);
            }
        );
    }
*/
    template<typename T>
    void deleteDeadObjects2(T& objects) noexcept {
        for( auto it = objects.begin(); it != objects.end(); ) {
            if(it->second.get()->isDead()) {
                if(it->second.get()->respawn()) {
                    ++it;
                } else {
                    it = objects.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
/*
    void deleteDeadObjects3() noexcept {
        for( auto it = entities_.begin(); it != entities_.end(); ) {
            if(EntityHelper::isDead(it->second.get())) {
                if(EntityHelper::respawn(it->second.get())) {
                    ++it;
                } else {
                    it = entities_.erase(it);
                }
            } else {
                ++it;
            }
        }
    }
*/
    template <typename S>
    GameObject* findGameObject(const std::map<uint32_t, std::unique_ptr<S>>& map, uint32_t gameObjectId) {
        auto it = map.find(gameObjectId);
        if(it != map.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    std::map<uint32_t, KnightUPtr>   knights_;
    std::map<uint32_t, ParticleUPtr> particles_;
    std::map<uint32_t, RollingStoneUPtr> rollingStones_;
    std::map<uint32_t, RPGBgElementUPtr> rpgBgElements_;
    std::map<uint32_t, BlueTriangleUPtr> otherElements_;
    //std::map<uint32_t, EntityInterfaceUPtr> entities_;
};

using WorldUPtr = std::unique_ptr<World>;
using WorldSPtr = std::shared_ptr<World>;

#endif // __WORLD_H__
