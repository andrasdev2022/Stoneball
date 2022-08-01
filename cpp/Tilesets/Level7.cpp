#include "Level7.h"
#include "LevelFactory.h"

void Level7::init(WorldUPtr& world) noexcept {
    tileMap_ =
            //01234567890123456789
            L"         ****    C  \n" // 0
             "   d   ***  *a      \n" // 0
             "       *  o *  C    \n" // 1
             " a   c * *o *t      \n" // 2
             "       *  o *       \n" // 3
             " s    a*x   *   S   \n" // 4
             "       *xxh**       \n" // 5
             "     d *****  c     \n" // 0
;
//            01234567890123456789

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level7::drawTexts() noexcept {
    Tileset::drawTexts(5, LevelFactory::minimumPushCount[4]);
}
