#ifndef __LEVEL4_H__
#define __LEVEL4_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level4 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level4TilesetUPtr = std::unique_ptr<Level4>;

#endif // __LEVEL4_H__
