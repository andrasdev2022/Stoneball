#include "Level31.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level31::init(WorldUPtr& world) noexcept {
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
    tileMap_ =
            //01234567890123
        L"----*****-*******---\n" // 0
         "---**  x*** xxo *---\n" // 1
         "---*o*  x**  *  *---\n" // 2
         "---* HxH ***H   *---\n" // 3
         "---*o H ****  *x*---\n" // 4
         "---* h  o HHo o *---\n" // 5
         "---***** *  *****---\n"
         "---*  o* **o H H*---\n" // 6
         "---** o o**o  x *---\n" // 7
         "---*    H***  * *---\n" // 8
         "---* xH*****oHxx*---\n" // 9
         "---*x H  x *    *---\n" // 10
         "---**************---\n" // 11
;
/*

cells from level23cpp_01.txt
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 24
                  g(): 315
                  depth: 26
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "**  x***\n" // 0
 "*o*  x**\n" // 1
 "* HxH **\n" // 2
 "*o H ***\n" // 3
 "* h  o *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RPQRUtLLLRQqLRDURUUPRURUR (25)
lines: 21
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 23
                  g(): 256
                  depth: 25
                  Teleport card count: 1
                  Push card count: 2
                  Pull card count: 5

//0123456789
 "* oxxh**\n" // 0
 "*  *  **\n" // 1
 "*   H***\n" // 2
 "*x*  ***\n" // 3
 "* o oHH*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DQDtLDLqUpUqDUDRUUPRRUUL (24)
lines: 20
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 316
                  depth: 27
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 5

//0123456789
 "*  o* **\n" // 0
 "** oho**\n" // 1
 "*    H**\n" // 2
 "* xH****\n" // 3
 "*x H  x*\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: LUQRQDDpDLDtLULDLpLpDDDRRR (26)
lines: 20
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 26
                  g(): 288
                  depth: 28
                  Teleport card count: 1
                  Push card count: 4
                  Pull card count: 4

//0123456789
 "*H H o**\n" // 0
 "* x  o**\n" // 1
 "* *  ***\n" // 2
 "*xxHo***\n" // 3
 "*h   * *\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: UtDLDDLpQDLpLLULDDLqUULDDLL (27)
lines: 20
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
    initDecks(world, 21, 12, 5);
}

Level31::Level31()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level31::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level31::drawTexts() noexcept {
    Tileset::drawTexts(29, LevelFactory::minimumPushCount[28]);
}
