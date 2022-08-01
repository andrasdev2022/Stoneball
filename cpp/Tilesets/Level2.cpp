#include "Level2.h"
#include "Application.h"
#include "Environment.h"
#include "MediaData.h"
#include "LevelFactory.h"

void Level2::init(WorldUPtr& world) noexcept {
    tileMap_ =
              L"     d    a    A    \n"   // 0
               "   **************   \n"   // 1
               "c *S      x xt  a*  \n"   // 2
               "  *    ***** lb  *A \n"   // 3
               "   *  *  a  *  T *- \n"   // 4
               "   * * *     **** - \n"   // 5
               " s *  o          a- \n"   // 6
               "   * * *   a   A  - \n"   // 7
               " A *  o*         d- \n"   // 8
               "   * * *d         - \n"   // 9
               "C  * S *          - \n"   // 10
               "    ***   B----d  - \n"   // 11
               "   - a     h    A - \n"   // 12
;
//              012345678901234567890

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
}

void Level2::drawTexts() noexcept {
    Tileset::drawTexts(1, LevelFactory::minimumPushCount[0]);
}
