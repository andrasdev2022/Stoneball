#ifndef __LEVEL14_H__
#define __LEVEL14_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level14 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level14TilesetUPtr = std::unique_ptr<Level14>;

#endif // __LEVEL14_H__
