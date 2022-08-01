#ifndef __LEVEL17_H__
#define __LEVEL17_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"
#include "TilesetCardHandler.h"

class Level17 : public Tileset, public TilesetCardHandler
{
public:
    Level17();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level17TilesetUPtr = std::unique_ptr<Level17>;

#endif // __LEVEL17_H__
