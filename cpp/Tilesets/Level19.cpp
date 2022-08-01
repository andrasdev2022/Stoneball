#include "Level19.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level19::init(WorldUPtr& world) noexcept {
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
           //0123456789012
           L"************ \n" // 0
            "*   x *  o * \n"
            "* Ho  *    * \n"
            "*x**  HHHH * \n"
            "***   *    **\n"
            "  *   * o  h*\n"
            "  **x *  ****\n"
            "   *******   \n" // 5
            //0123456789
;
//              ?? states, 35 pushes

    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    // pull push teleport
    initDecks(world, 2, 10, 0);
}

Level19::Level19()
: Tileset()
, TilesetCardHandler(1) {

}

void Level19::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level19::drawTexts() noexcept {
    Tileset::drawTexts(17, LevelFactory::minimumPushCount[16]);
}
