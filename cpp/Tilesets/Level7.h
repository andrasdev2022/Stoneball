#ifndef __LEVEL7_H__
#define __LEVEL7_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level7 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level7UPtr = std::unique_ptr<Level7>;

#endif // __LEVEL7_H__
