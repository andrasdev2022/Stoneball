#include "Level18.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level18::init(WorldUPtr& world) noexcept {

    std::wstring firstLayer =
            //01234567890123456789
            L" a               \n" // 0
             " aa              \n" // 1
             "                 \n" // 2
             "            A    \n" // 3
             "d   c          A \n" // 4
             "             A   \n" // 5
             "  d              \n" // 6
            //01234567890123456789
;
    tileMap_ =
           //01234567890123456789
           L"-----*******-----\n" // 0
            "-----* x   *-----\n" // 1
            "-----**o  o*-----\n" // 2
            "-----*x** H*-----\n" // 3
            "-----*x Hoh*-----\n" // 4
            "-----*  *  *-----\n" // 5
            "-----*******-----\n" // 6
           //01234567890123456789
;
//              52 states, 27 pushes

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    // pull push teleport
    initDecks(world, 4, 4, 1);
}

Level18::Level18()
: Tileset()
, TilesetCardHandler(1) {

}

void Level18::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level18::drawTexts() noexcept {
    Tileset::drawTexts(16, LevelFactory::minimumPushCount[15]);
}
