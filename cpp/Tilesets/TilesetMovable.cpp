#include "TilesetMovable.h"
#include "BouncingStones.h"
#include "Application.h"
#include "Screen.h"
#include "Environment.h"
#include "Global.h"

TilesetMovable::TilesetMovable()
: Tileset()
{

}

void TilesetMovable::followTheKnight() noexcept {
    // I'll create a dummy game object in order to reposition the screen
    int positionY = position().y;
    if(heightInPixels() > (int)screenHeight()) {
        positionY += heightInPixels() / 2;
    } else {
        positionY += screenHeight() / 2;
    }
    int positionX = position().x;
    if(widthInPixels() > (int)screenWidth()) {
        positionX += widthInPixels() / 2;
    } else {
        positionX += screenWidth() / 2;
    }
    GrayBouncingStone gbs(positionX, positionY);
    gbs.setX(gbs.x() - gbs.width() / 2 + deltaX_);
    gbs.setY(gbs.y() - gbs.height() / 2 + deltaY_);
    Application::screen()->follow(&gbs, Application::environment()->gameMap());
}
