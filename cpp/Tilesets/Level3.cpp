#include "Level3.h"

void Level3::init(WorldUPtr& world) noexcept {
    std::wstring firstLayer =
              L"     d    a    A    \n"   // 0
               "                    \n"   // 1
               "   S  ┌────     a   \n"   // 2
               "      |           A \n"   // 3
               "      |  a          \n"   // 4
               "      |             \n"   // 5
               "      |          a  \n"   // 6
               "      |    a   A    \n"   // 7
               " A    |          d  \n"   // 8
               "      | d           \n"   // 9
               "     S|             \n"   // 10
               "      └─────R  d    \n"   // 11
               "     a          A   \n"   // 12
;
//              0123456789012
    tileMap_ =
              L"                    \n"   // 0
               " **************     \n"   // 1
               "c*        x   *     \n"   // 2
               " *            *     \n"   // 3
               " ***** ********     \n"   // 4
               "                    \n"   // 5
               " s o    T           \n"   // 6
               "                    \n"   // 7
               "                    \n"   // 8
               "              t     \n"   // 9
               "C                   \n"   // 10
               "                    \n"   // 11
               "           h        \n"   // 12
;
//              0123456789012

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level3::drawTexts() noexcept {
    Tileset::drawTexts(0, 0);
}
