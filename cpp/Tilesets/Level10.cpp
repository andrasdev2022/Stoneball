#include "Level10.h"
#include "LevelFactory.h"

void Level10::init(WorldUPtr& world) noexcept {
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
           L"     A********     \n" // 0
            "      *xxxxx *     \n" // 1
            "   a  * * *  * c   \n" // 2
            "      ***** **     \n" // 3
            "      *  o   *     \n" // 4
            "  d   *  o o *  s  \n" // 5
            "      * *o o**     \n" // 6
            "   C  *   h *    S \n" // 7
            "      *******      \n" // 8
;
//              127 állapot

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level10::drawTexts() noexcept {
    Tileset::drawTexts(8, LevelFactory::minimumPushCount[7]);
}
