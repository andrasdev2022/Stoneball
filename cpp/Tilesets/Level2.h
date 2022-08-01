#ifndef __LEVEL2_H__
#define __LEVEL2_H__

#include <string>
#include "World.h"
#include "Tileset.h"

class Level2 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level2UPtr = std::unique_ptr<Level2>;

#endif // __LEVEL2_H__
