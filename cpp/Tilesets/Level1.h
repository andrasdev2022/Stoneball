#ifndef __LEVEL1_H__
#define __LEVEL1_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level1 : public Tileset
{
    using Tileset::Tileset;
    friend class LevelFctory;
public:

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level1TilesetUPtr = std::unique_ptr<Level1>;

#endif // __LEVEL1_H__
