#include "Level15.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level15::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
            //01234567890123456789
            L"     ---------     \n" // 0
             "  c  -       - d   \n" // 1
             "     -       -     \n" // 2
             "    a--   ----  C  \n" // 3
             "     --- -----     \n" // 4
             "     -   -   -  A  \n" // 5
             "  A  -  -    -     \n" // 6
             "     -       - d   \n" // 7
             "     -       -     \n" // 8
             "     ---------     \n" // 9
;
    tileMap_ =
           //01234567890123456789
           L"-----*********-----\n" // 0
            "-----*       *-----\n" // 1
            "-----*       *-----\n" // 2
            "-----** o ****-----\n" // 3
            "-----***H*****-----\n" // 4
            "-----*  h*x  *-----\n" // 5
            "-----*  *    *-----\n" // 6
            "-----*       *-----\n" // 7
            "-----*       *-----\n" // 8
            "-----*********-----\n" // 9
;
//              30 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    initDecks(world, 2, 2, 2);
}

Level15::Level15()
: Tileset()
, TilesetCardHandler(1) {

}

void Level15::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level15::drawTexts() noexcept {
    Tileset::drawTexts(13, LevelFactory::minimumPushCount[12]);
}
