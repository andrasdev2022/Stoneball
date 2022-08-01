#ifndef __LEVEL26_H__
#define __LEVEL26_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "TilesetMovable.h"
#include "TilesetCardHandler.h"

class Level26 : public TilesetMovable, public TilesetCardHandler
{
public:
    Level26();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level26TilesetUPtr = std::unique_ptr<Level26>;

#endif // __LEVEL26_H__
