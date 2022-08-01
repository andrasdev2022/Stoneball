#include "Level6.h"
#include "LevelFactory.h"

void Level6::init(WorldUPtr& world) noexcept {
//              0123456789012345678
    tileMap_ =
              L"     d****** d d   \n"   // 0
               " d    *    *    c  \n"   // 1
               "  d   * *o *       \n"   // 2
               "   a  *  o *  s    \n"   // 3
               "       *   *       \n"   // 4
               "      *xxh*    S d \n"   // 5
               "  C    ***  d   d  \n"   // 6
;
//              0123456789012345678

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level6::drawTexts() noexcept {
    Tileset::drawTexts(4, LevelFactory::minimumPushCount[3]);
}
