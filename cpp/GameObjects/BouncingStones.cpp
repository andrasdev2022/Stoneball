#include "BouncingStones.h"
#include "stoneball.h"
#include "GameObjectDescriptor.h"
#include <math.h>
#include "MediaData.h"
#include <iostream>
#include "Algorithms.h"

RedBouncingStone::RedBouncingStone(double x, double bottomY) noexcept
    : BlueTriangle(x, bottomY, GameObject::Type::GOT_RED_BOUNCING_STONE)
{
}

BlueTriangle* RedBouncingStone::copy() const noexcept {
    return new RedBouncingStone(*this);
}

GrayBouncingStone::GrayBouncingStone(double x, double bottomY) noexcept
    : BlueTriangle(x, bottomY, GameObject::Type::GOT_GRAY_BOUNCING_STONE)
{
}

BlueTriangle* GrayBouncingStone::copy() const noexcept {
    return new GrayBouncingStone(*this);
}

YellowBouncingStone::YellowBouncingStone(double x, double bottomY) noexcept
    : BlueTriangle(x, bottomY, GameObject::Type::GOT_YELLOW_BOUNCING_STONE)
{
}

BlueTriangle* YellowBouncingStone::copy() const noexcept {
    return new YellowBouncingStone(*this);
}

GreenBouncingStone::GreenBouncingStone(double x, double bottomY) noexcept
    : BlueTriangle(x, bottomY, GameObject::Type::GOT_GREEN_BOUNCING_STONE)
{
}

BlueTriangle* GreenBouncingStone::copy() const noexcept {
    return new GreenBouncingStone(*this);
}
