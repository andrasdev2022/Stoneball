#ifndef __FIREWORKS_H__
#define __FIREWORKS_H__

#include "BlueTriangle.h"

class Fireworks : public BlueTriangle
{
public:
    Fireworks(double x, double bottomY, GameObject::Type type) noexcept;

    BlueTriangle* copy() const noexcept override;

    // GameObject
    void update(double now) noexcept override;
    const GameObjectDescriptor& god() const noexcept override;
    double z() const noexcept override {return 10000.;}

    Fireworks* fireworks() noexcept override {return this;}
};

using FireworksUPtr = std::unique_ptr<Fireworks>;
#endif // __FIREWORKS_H__
