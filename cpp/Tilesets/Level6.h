#ifndef __LEVEL6_H__
#define __LEVEL6_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level6 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level6TilesetUPtr = std::unique_ptr<Level6>;

#endif // __LEVEL6_H__
