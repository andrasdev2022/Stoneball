#ifndef __GAMEOBJECTCOLLIDER_H__
#define __GAMEOBJECTCOLLIDER_H__

#include "Knight.h"
#include "RollingStone.h"
#include "CollisionData.h"
#include "Collision.h"

#include <map>

class World;

class GameObjectCollider final
{
public:
    GameObjectCollider(World* world) : world_(world) {}

    bool testHeroStoneCollisions(KnightUPtr& hero) noexcept;
    bool testStoneCollisions(const RollingStoneUPtr& stone) noexcept;
    bool testHeroRPGBgElementCollisions(const KnightUPtr& hero) noexcept;
    bool testHeroHeroCollisions(KnightUPtr& hero) noexcept;

private:

    template <typename S>
    bool testOneManyGameObjectCollision(GameObject* gameObject, const std::map<uint32_t, std::unique_ptr<S>>& map);

    bool isCollisionAlloved(GameObject* g1, GameObject* g2) const noexcept;

    World* world_{};
    GameObject* collidedObject_;
};


template <typename S>
bool GameObjectCollider::testOneManyGameObjectCollision(GameObject* gameObject, const std::map<uint32_t, std::unique_ptr<S>>& map) {
    collidedObject_ = nullptr;
    if(!gameObject || !gameObject->collisionData()) return false;

    int gameObjectX = gameObject->x();
    int gameObjectY = gameObject->y();
    const Rect gameObjectRect = gameObject->collisionData()->rect();
    for( auto it = map.begin(); it != map.end(); ++it) {
        if(gameObject->id() == it->second->id()) continue;
        if(it->second->isDead() || !it->second->collisionData() || it->second->collisionData()->rect().isNull()) continue;
        int elementX = it->second->x();
        int elementY = it->second->y();

        const Rect elementRect = it->second->collisionData()->rect();
        if(isCollisionAlloved(gameObject, it->second.get())
        && Collision::detectRectCollision(gameObjectX, gameObjectY, gameObjectRect, elementX, elementY, elementRect, 0, 0))
        {
            collidedObject_ = it->second.get();
            return true;
        }
    }
    return false;
}

#endif // __GAMEOBJECTCOLLIDER_H__
