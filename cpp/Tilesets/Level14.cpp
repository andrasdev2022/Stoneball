#include "Level14.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level14::init(WorldUPtr& world) noexcept {
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
           //01234567890123456789
           L"    c*********     \n" // 0
            "  a  *xxxxxxx* A   \n" // 1
            "     *  x x  *     \n" // 2
            "    C***    **     \n" // 3
            "     *  ooo  *     \n" // 4
            "  d  * oo oo *  A  \n" // 5
            "     *  o o  *     \n" // 6
            "     *   h * *     \n" // 7
            "    d*     * *A    \n" // 8
            "     *********     \n" // 9
;
//              127 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level14::drawTexts() noexcept {
    Tileset::drawTexts(12, LevelFactory::minimumPushCount[11]);
}
