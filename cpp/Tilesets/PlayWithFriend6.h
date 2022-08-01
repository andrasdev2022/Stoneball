#ifndef __PLAYWITHFRIEND6_H__
#define __PLAYWITHFRIEND6_H__

#include "PlayWithFriend5.h"
#include "TilesetMovable.h"

class PlayWithFriend6 : public PlayWithFriend5, public TilesetMovable
{
public:
    PlayWithFriend6();
    void init(WorldUPtr& world) noexcept override;
    void followTheKnight() noexcept override {TilesetMovable::followTheKnight();}
};

#endif // __PLAYWITHFRIEND6_H__
