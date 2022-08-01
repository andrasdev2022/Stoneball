#ifndef __LEVEL13_H__
#define __LEVEL13_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level13 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level13UPtr = std::unique_ptr<Level13>;

#endif // __LEVEL13_H__
