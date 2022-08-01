#include "Level5.h"
#include "LevelFactory.h"

void Level5::init(WorldUPtr& world) noexcept {
    tileMap_ =
    //0123456789
    L"  t  ********     \n" // 0
     "     *   ****  t  \n" // 0
     " s   *  o  **     \n" // 1
     "     **oh * *   c \n" // 2
     "a d  *  *  x*     \n" // 3
     "     ** x   *S    \n" // 4
     "     *  **  *     \n" // 5
     " C   ********  t  \n" // 5
    // steps: LRDDRDDLUURDRDDRRU (18)
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

void Level5::drawTexts() noexcept {
    Tileset::drawTexts(3, LevelFactory::minimumPushCount[2]);
}
