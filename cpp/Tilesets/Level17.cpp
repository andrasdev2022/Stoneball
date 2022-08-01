#include "Level17.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level17::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
            //01234567890123456789
            L" C               \n" // 0
             "               a \n" // 1
             "  C              \n" // 2
             "             c c \n" // 3
             "    d         c  \n" // 4
             "                 \n" // 5
             "              C  \n" // 6
            //01234567890123456789
;
    tileMap_ =
           //01234567890123456789
           L"-----*******-----\n" // 0
            "-----* o  **-----\n" // 1
            "-----* *H x*-----\n" // 2
            "-----*h*  x*-----\n" // 3
            "-----* *o***-----\n" // 4
            "-----*    **-----\n" // 5
            "-----*******-----\n" // 6
           //01234567890123456789
;
//          110 állapot, 7 walls, push count 21
//          Vince: 11 min, 8/10

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    initDecks(world, 4, 4, 1);
}

Level17::Level17()
: Tileset()
, TilesetCardHandler(1) {

}

// textXPos_ is the position reference for deck of cards too
// calculation is in the TilesetCardHandler
void Level17::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width_;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level17::drawTexts() noexcept {
    Tileset::drawTexts(15, LevelFactory::minimumPushCount[14]);
}
