#include "Level9.h"
#include "LevelFactory.h"

void Level9::init(WorldUPtr& world) noexcept {
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
           //012345678901234567890
           L"      c******     C  \n" // 0
            "       *xxx ***      \n" // 1
            "  A    *x   o * a    \n" // 2
            "       *x* *o *   d  \n" // 3
            "       ***  o *      \n" // 4
            "   d     ***o * d    \n" // 5
            "         *  o *    d \n" // 6
            "         * h  *      \n" // 7
            "         ******t     \n" // 8
;
//              119 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level9::drawTexts() noexcept {
    Tileset::drawTexts(7, LevelFactory::minimumPushCount[6]);
}
