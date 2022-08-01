#include "Level30.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level30::init(WorldUPtr& world) noexcept {
    std::wstring decorationLayer =
        //01234567890123456789
        L"                     \n" // 0
         "                     \n" // 1
         "                     \n" // 2
         "                     \n" // 3
         "                     \n" // 4
         "                     \n" // 5
         "                     \n" // 6
         "                     \n" // 7
         "                     \n" // 8
         "                     \n" // 9
         "                     \n" // 10
         "                     \n" // 11
         "                     \n" // 12
;

    tileMap_ =
            //01234567890123
        L"---***************---\n" // 0
         "---**oH    *     *---\n" // 1
         "---*oxx  o*   o o*---\n" // 2
         "---*  **o * H**  *---\n" // 3
         "---* H** x*  **o *---\n" // 4
         "---*  H x**xxx  H*---\n" // 5
         "---* * * * * * * *---\n" // 6
         "---** H x  *  H  *---\n" // 7
         "---* oo   *      *---\n" // 8
         "---*  **  *oo**H *---\n" // 9
         "---*oH** x*h **ox*---\n" // 10
         "---*   x  * x  x *---\n" // 11
         "---***************---\n" // 12
;
/*
cell level22cpp: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 31
                  g(): 292
                  depth: 32
                  Teleport card count: 0
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "**oH   *\n" // 0
 "*oxxh o*\n" // 1
 "*  **o *\n" // 2
 "* H** x*\n" // 3
 "*  H x**\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: pLLqLLLRDQRDQRRRDURDUPDURRRRDDD (31)
lines: 20
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 33
                  g(): 352
                  depth: 34
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "**     *\n" // 0
 "*   o o*\n" // 1
 "* H**  *\n" // 2
 "*  **o *\n" // 3
 "*xxxh H*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UUULpURRRQtLLLLQDQRLDDpDLDDLLDDDR (33)
lines: 20
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 30
                  g(): 234
                  depth: 32
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 3

//0123456789
 "** H x *\n" // 0
 "* oo   *\n" // 1
 "*  **  *\n" // 2
 "*oH** x*\n" // 3
 "* h x  *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UUDQRUDRRtUQRRRDURDUPDURRRRDDDL (31)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 38
                  g(): 270
                  depth: 39
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 4

//0123456789
 "**  H  *\n" // 0
 "*      *\n" // 1
 "*oo**H *\n" // 2
 "*h **ox*\n" // 3
 "* x  x *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UUDQRRLDDDUUPDDDRLRRDtLLLLqUUPDURRRRDD (38)
lines: 21
--------------------------------------------------------------------------------
*/
    int row = 0;
    int col = 0;
    calcTileMapRect();
    std::for_each(decorationLayer.begin(), decorationLayer.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    row = 0;
    col = 0;
    std::for_each(tileMap_.begin(), tileMap_.end(), [&](wchar_t& c){initTile(c, row, col, world);});
    setupStonHoleCollision(world);
    // pull push teleport
    initDecks(world, 11, 4, 2);
}

Level30::Level30()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level30::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level30::drawTexts() noexcept {
    Tileset::drawTexts(28, LevelFactory::minimumPushCount[27]);
}
