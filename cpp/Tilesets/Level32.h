#ifndef __LEVEL32_H__
#define __LEVEL32_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "TilesetMovable.h"
#include "TilesetCardHandler.h"

class Level32 : public TilesetMovable, public TilesetCardHandler
{
public:
    Level32();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level32TilesetUPtr = std::unique_ptr<Level32>;

#endif // __LEVEL32_H__
