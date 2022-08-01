#ifndef __LEVEL3_H__
#define __LEVEL3_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level3 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level3TilesetUPtr = std::unique_ptr<Level3>;

#endif // __LEVEL3_H__
