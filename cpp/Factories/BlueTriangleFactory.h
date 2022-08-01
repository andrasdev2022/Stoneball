#ifndef __BLUETRIANGLEFACTORY_H__
#define __BLUETRIANGLEFACTORY_H__

#include "BlueTriangle.h"

class BlueTriangleFactory
{
public:
    static BlueTriangleUPtr create(GameObject::Type type) noexcept;
};

#endif // BLUETRIANGLEFACTORY_H
