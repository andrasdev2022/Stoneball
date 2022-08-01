#ifndef __LEVEL5_H__
#define __LEVEL5_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level5 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level5TilesetUPtr = std::unique_ptr<Level5>;

#endif // __LEVEL5_H__
