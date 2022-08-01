#include "KnightFactory.h"
#include "Application.h"
#include "MediaData.h"
#include "CollisionData.h"
#include "Environment.h"

namespace KnightFactory {

KnightUPtr create(GameObject::Type type) noexcept {
    //std::cout << "KnightFactory::create " << static_cast<int>(type) << "\n";
    KnightUPtr knight = std::make_unique<Knight>(0._ZRW, 32._ZRH, type);
    // attack1, attack2: 6 frame, attack3: 8 frame
    knight->setDamagePerFrame(48); // damage per attack or hit / hero attack frame count (not all of them should hurt)
    knight->setHitPoints(4000, 4000);
    //hero_->setHitPoints(200000, 200000);
    knight->setMaxHeightOfJump(200._ZRD);
    CollisionData* collisionData = new CollisionData(Rect(8_ZRD, 20_ZRD, 16_ZRD, 12_ZRD));
    knight->setCollisionData(collisionData);
    return knight;
}

} // namespace KnightFactory
