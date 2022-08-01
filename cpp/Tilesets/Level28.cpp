#include "Level28.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level28::init(WorldUPtr& world) noexcept {
    std::wstring decorationLayer =
        //01234567890123456789
        L"                    \n" // 0
         "                    \n" // 1
         "                    \n" // 2
         "                    \n" // 3
         "                    \n" // 4
         "                    \n" // 5
         "                    \n" // 6
         "                    \n" // 7
         "                    \n" // 8
         "                    \n" // 9
         "                    \n" // 10
         "                    \n" // 11
;
//              0123456789012
    tileMap_ =
            //01234567890123
        L"---**************---\n" // 0
         "---** H  *xx   **---\n" // 1
         "---*xx  H**x  ***---\n" // 2
         "---* xH **HH H o*---\n" // 3
         "---*   oo*   o  *---\n" // 4
         "---*** o ** ** o*---\n" // 5
         "---* x****x**  **---\n" // 6
         "---**o   *x hoH *---\n" // 7
         "---*    *o   H o*---\n" // 8
         "---*o H H *  *xo*---\n" // 9
         "---*  H xx**H   *---\n" // 10
         "---**************---\n" // 11
;
/*
cell from level21cpp_00.txt: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 24
                  g(): 285
                  depth: 25
                  Teleport card count: 0
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "** H h**\n" // 0
 "*xx  H**\n" // 1
 "* xH ***\n" // 2
 "*   oo**\n" // 3
 "*** o **\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: qqLDLqUpLLUQLLULDUDLULUL (24)
lines: 20
--------------------------------------------------------------------------------
cell from level21cpp_01.txt: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 27
                  g(): 288
                  depth: 28
                  Teleport card count: 0
                  Push card count: 5
                  Pull card count: 4

//0123456789
 "* x*****\n" // 0
 "**o h **\n" // 1
 "*    *o*\n" // 2
 "*o H H *\n" // 3
 "*  H xx*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: ULPLRRDQRDRDRDRRULUQpLLLUUU (27)
lines: 20
--------------------------------------------------------------------------------
cell from level21cpp_10.txt: 10
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 29
                  g(): 261
                  depth: 30
                  Teleport card count: 0
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "*xx   **\n" // 0
 "**x  ***\n" // 1
 "*HH H o*\n" // 2
 "*   o  *\n" // 3
 "**h** o*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RUUDpLLULDqpLqDULLULURULLLUUL (29)
lines: 22
--------------------------------------------------------------------------------
cell from level21cpp_11.txt: 11
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 28
                  g(): 318
                  depth: 29
                  Teleport card count: 0
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "*x**  **\n" // 0
 "*x hoH *\n" // 1
 "*   H o*\n" // 2
 "**  *xo*\n" // 3
 "***H   *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: pQRURRDDLURUppULLULpDULLLULU (28)
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
    initDecks(world, 17, 14, 2);
}

Level28::Level28()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level28::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level28::drawTexts() noexcept {
    Tileset::drawTexts(26, LevelFactory::minimumPushCount[25]);
}
