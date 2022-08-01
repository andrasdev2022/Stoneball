#ifndef __RPGBGELEMENT_H__
#define __RPGBGELEMENT_H__

#include "GameObject.h"
#include "GameObjectDescriptor.h"

class StaticObjects;

class RPGBgElement final : public GameObject {
public:
    RPGBgElement(double x, double y, int bitmapId) noexcept;
    RPGBgElement(const RPGBgElement& g) noexcept = default;

    // GameObject
    const GameObjectDescriptor& god() const noexcept override;
    void update(double) override;
    double z() const noexcept override;
    void die() noexcept override {dead_ = true;}
    bool isDead() const noexcept override {return dead_;}

    // SerializeInterface
    bool addData(Serialize*) override;
    // copy data into the UDPMessageData, it will be synchronized by SnapshotSyncher
    // and MultiplayerWorld
    static const uint8_t* buildFrom(UDPMessageData*, const uint8_t*);

    void changeBitmapId(int bitmapId) noexcept;

private:
    GameObjectDescriptor descriptor_;
    bool dead_{false};
};

using RPGBgElementUPtr = std::unique_ptr<RPGBgElement>;
using RPGBgElementSPtr = std::shared_ptr<RPGBgElement>;
#endif // __RPGBGELEMENT_H__
