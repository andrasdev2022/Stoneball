#include "Level16.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level16::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
            //01234567890123456789
            L"  d              \n" // 0
             "              a  \n" // 1
             " d d         a   \n" // 2
             "               a \n" // 3
             "  d              \n" // 4
             "              c  \n" // 5
             "                 \n" // 6
            //01234567890123456789
;
    tileMap_ =
            //01234567890123456789
            L"------******-----\n" // 0
             "-----**o   *-----\n" // 1
             "-----* H* x*-----\n" // 2
             "-----*h   x*-----\n" // 3
             "-----***o* *-----\n" // 4
             "-------*   *-----\n" // 5
             "-------*****-----\n" // 6
            //01234567890123456789
;
//              70 állapot
//              7 min, 6-7/10

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    initDecks(world, 3, 2, 1);
}

Level16::Level16()
: Tileset()
, TilesetCardHandler(1) {

}

void Level16::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level16::drawTexts() noexcept {
    Tileset::drawTexts(14, LevelFactory::minimumPushCount[13]);
}
