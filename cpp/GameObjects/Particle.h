#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "GameObject.h"

class StaticObjects;

class Particle final : public GameObject {
public:
    Particle(double x, double y, GameObject::Type type) noexcept;
    Particle(const Particle& g) noexcept = default;

    // GameObject
    void update(double now) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;
    void die() noexcept override;
    bool isDead() const noexcept override;

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

private:
    double bornAt{-1e+6};
};

using ParticleUPtr = std::unique_ptr<Particle>;
#endif // __PARTICLE_H__
