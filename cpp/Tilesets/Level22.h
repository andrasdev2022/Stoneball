#ifndef __LEVEL22_H__
#define __LEVEL22_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "TilesetMovable.h"
#include "TilesetCardHandler.h"

class Level22 : public TilesetMovable, public TilesetCardHandler
{
public:
    Level22();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level22TilesetUPtr = std::unique_ptr<Level22>;

#endif // __LEVEL22_H__
