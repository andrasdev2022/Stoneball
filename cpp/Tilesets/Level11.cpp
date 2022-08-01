#include "Level11.h"
#include "LevelFactory.h"

void Level11::init(WorldUPtr& world) noexcept {
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
           //0123456789012345678
           L"  a   *******  a   \n" // 0
            "      *xxx* *      \n" // 1
            "     A*x    * a  A \n" // 2
            "      *x  * **     \n" // 3
            "  a c ***  o * d   \n" // 4
            "      *  o*o *   d \n" // 5
            "    d *  o o * d   \n" // 6
            "     d** *h  *     \n" // 7
            "  d    *******d    \n" // 8
;
//              148 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level11::drawTexts() noexcept {
    Tileset::drawTexts(9, LevelFactory::minimumPushCount[8]);
}
