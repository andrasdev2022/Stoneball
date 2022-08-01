#ifndef __TILESETMOVABLE_H__
#define __TILESETMOVABLE_H__

#include "Tileset.h"

class TilesetMovable : virtual public Tileset
{
public:
    TilesetMovable();
    void move(int deltaX, int deltaY) noexcept override {deltaX_ = deltaX; deltaY_ = deltaY;}
    int deltaX() const noexcept override {return deltaX_;}
    int deltaY() const noexcept override {return deltaY_;}

    void followTheKnight() noexcept override;

private:
    int deltaX_{};
    int deltaY_{};
};

#endif // __TILESETMOVABLE_H__
