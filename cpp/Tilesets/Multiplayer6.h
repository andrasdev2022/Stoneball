#ifndef __MULTIPLAYER6_H__
#define __MULTIPLAYER6_H__

#include "Multiplayer5.h"
#include "TilesetMovable.h"

class Multiplayer6 : public Multiplayer5, public TilesetMovable
{
public:
    Multiplayer6();
    void init(WorldUPtr& world) noexcept override;
    void followTheKnight() noexcept override {TilesetMovable::followTheKnight();}
};

#endif // __MULTIPLAYER6_H__
