#ifndef __LEVEL11_H__
#define __LEVEL11_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level11 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level11TilesetUPtr = std::unique_ptr<Level11>;

#endif // __LEVEL11_H__
