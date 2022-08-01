#ifndef __LEVEL9_H__
#define __LEVEL9_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"

class Level9 : public Tileset
{
public:
    using Tileset::Tileset;

    void init(WorldUPtr& world) noexcept override;
    void drawTexts() noexcept override;
};

using Level9UPtr = std::unique_ptr<Level9>;

#endif // __LEVEL9_H__
