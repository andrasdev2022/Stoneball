#ifndef __LEVEL19_H__
#define __LEVEL19_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"
#include "TilesetCardHandler.h"

class Level19 : public Tileset, public TilesetCardHandler
{
public:
    Level19();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level19TilesetUPtr = std::unique_ptr<Level19>;

#endif // __LEVEL19_H__
