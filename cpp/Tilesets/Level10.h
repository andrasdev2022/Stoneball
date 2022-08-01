#ifndef __LEVEL10_H__
#define __LEVEL10_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level10 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level10TilesetUPtr = std::unique_ptr<Level10>;

#endif // __LEVEL10_H__
