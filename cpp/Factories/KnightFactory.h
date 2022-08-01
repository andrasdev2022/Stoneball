#ifndef __KNIGHTFACTORY_H__
#define __KNIGHTFACTORY_H__

#include "Knight.h"

namespace KnightFactory
{
    KnightUPtr create(GameObject::Type type) noexcept;
}

#endif // __KNIGHTFACTORY_H__
