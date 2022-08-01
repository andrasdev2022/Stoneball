#include "Level4.h"
#include "LevelFactory.h"

void Level4::init(WorldUPtr& world) noexcept {
    tileMap_ =
            //01234567890123456789
            L"    S*******      \n" // 0
             "  d  * x   *  c   \n" // 0
             "     ** ** *      \n" // 1
             " a   *     **     \n" // 2
             "     *   *oh*  A  \n" // 3
             "  t  *x*  o *   d \n" // 4
             "     *    ***     \n" // 5
             "d   C****** s     \n" // 6
            // steps: LUULUULUUDDDLLUULDLD (20)
            // lines: 12
;
    int row = 0;
    int col = 0;
    calcTileMapRect();
    //std::for_each(firstLayer.begin(), firstLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    //row = 0;
    //col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level4::drawTexts() noexcept {
    Tileset::drawTexts(2, LevelFactory::minimumPushCount[1]);
}
