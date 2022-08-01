#include "Level20.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level20::init(WorldUPtr& world) noexcept {
//              0123456789012
/*    std::wstring firstLayer =
            L"       \n"   // 0
             "  ╔═╗  \n"   // 1
             " ╔32╝  \n"   // 2
             " ╚1»   \n"   // 3
             "  «4╗  \n"   // 4
             "  ╚#╝  \n"   // 5
             "       \n"   // 6
;*/
//              0123456789012
    tileMap_ =
            //01234567890123
            L"**************\n" // 0
             "**xxx     Hx *\n" // 0
             "* H *hHH    o*\n" // 1
             "*oHHo *  xHHH*\n" // 2
             "*H H  *   Ho *\n" // 3
             "**   o*    xo*\n" // 4
             "-*************\n" // 5
            //01234567890123
;
//              ?? states, 43 pushes

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    // pull push teleport
    initDecks(world, 8, 8, 2);
}

Level20::Level20()
: Tileset()
, TilesetCardHandler(1) {

}

void Level20::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level20::drawTexts() noexcept {
    Tileset::drawTexts(18, LevelFactory::minimumPushCount[17]);
}
