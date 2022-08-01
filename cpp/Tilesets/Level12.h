#ifndef __LEVEL12_H__
#define __LEVEL12_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level12 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level12TilesetUPtr = std::unique_ptr<Level12>;

#endif // __LEVEL12_H__
