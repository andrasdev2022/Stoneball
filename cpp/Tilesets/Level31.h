#ifndef __LEVEL31_H__
#define __LEVEL31_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "TilesetMovable.h"
#include "TilesetCardHandler.h"

class Level31 : public TilesetMovable, public TilesetCardHandler
{
public:
    Level31();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level31TilesetUPtr = std::unique_ptr<Level31>;

#endif // __LEVEL31_H__
