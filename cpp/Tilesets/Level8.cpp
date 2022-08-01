#include "Level8.h"
#include "LevelFactory.h"

void Level8::init(WorldUPtr& world) noexcept {
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
            L" d c *x*      C   \n" // 0
             "     *x******   C \n" // 1
             "   a *x     *     \n" // 2
             "     * *  o * a   \n" // 3
             " C   * ***o *     \n" // 4
             "   a * *  o *A    \n" // 5
             "     *   h  *     \n" // 6
             "   d ********  A  \n" // 7
;
//              85 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level8::drawTexts() noexcept {
    Tileset::drawTexts(6, LevelFactory::minimumPushCount[5]);
}
