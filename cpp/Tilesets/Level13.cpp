#include "Level13.h"
#include "LevelFactory.h"

void Level13::init(WorldUPtr& world) noexcept {
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
           L"     *********  d  \n" // 0
            "  a  *    ** *     \n" // 1
            "     **   o  *     \n" // 2
            "   a ** ***o *  A  \n" // 3
            "     *hxxx*  *     \n" // 4
            " C   * xxx*o *     \n" // 5
            "     * ****  *     \n" // 6
            "   d * o o o *     \n" // 7
            "     *       * A   \n" // 8
            "     *********     \n" // 9
;
//              197 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level13::drawTexts() noexcept {
    Tileset::drawTexts(11, LevelFactory::minimumPushCount[10]);
}
