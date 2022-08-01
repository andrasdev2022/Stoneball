#ifndef __LEVEL15_H__
#define __LEVEL15_H__

#include <string>
#include "Size.h"
#include "Position.h"
#include "World.h"
#include "Tileset.h"
#include "TilesetCardHandler.h"

class Level15 : public Tileset, public TilesetCardHandler
{
public:
    Level15();

    void init(WorldUPtr& world) noexcept override;
    void update(double now) noexcept override {Tileset::update(now); updateDecks(now); updateCards(now);}

    virtual TilesetCardHandler* tilesetCardHandler() noexcept override {return this;}
    virtual const TilesetCardHandler* tilesetCardHandler() const noexcept override {return this;}

    void updateTexts(double now) noexcept override;
    void drawTexts() noexcept override;
};

using Level15TilesetUPtr = std::unique_ptr<Level15>;

#endif // __LEVEL15_H__
