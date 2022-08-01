#include "Level12.h"
#include "LevelFactory.h"

void Level12::init(WorldUPtr& world) noexcept {
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
            L"  c   ********     \n" // 0
             "     C*xxx   *  c  \n" // 1
             "      *x   * *     \n" // 2
             "   d  *x* o  *a    \n" // 3
             "      *x ooo *     \n" // 4
             "  d   ** oho * A   \n" // 5
             "    A  *** * *     \n" // 6
             "   a     *   *   a \n" // 7
             "     A   *****     \n" // 8
;
//              148 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level12::drawTexts() noexcept {
    Tileset::drawTexts(10, LevelFactory::minimumPushCount[9]);
}
