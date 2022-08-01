#ifndef __LEVEL8_H__
#define __LEVEL8_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level8 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level8UPtr = std::unique_ptr<Level8>;

#endif // __LEVEL8_H__
