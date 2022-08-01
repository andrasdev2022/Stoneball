#include "Level29.h"
#include "BitmapIds.h"
#include "LevelFactory.h"

void Level29::init(WorldUPtr& world) noexcept {
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
        L"---**************---\n" // 0
         "---* o x  * *x o*---\n" // 1
         "---** HhH* * x  *---\n" // 2
         "---* xHo * HH H**---\n" // 3
         "---*x   **  o **----\n" // 4
         "---*o ****xo ***----\n" // 5
         "---*H  x x*o * **---\n" // 6
         "---*   * H H  xx*---\n" // 7
         "---* H ***  H  **---\n" // 8
         "---*ox o**Ho  **----\n" // 9
         "---*o  ***o x**-----\n" // 10
         "---***********------\n" // 11
;
/*
cell from level26cpp_00.txt: 00
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 22
                  g(): 342
                  depth: 24
                  Teleport card count: 1
                  Push card count: 5
                  Pull card count: 5

//0123456789
 "* o x  *\n" // 0
 "** HhH**\n" // 1
 "* xHo **\n" // 2
 "*x   ***\n" // 3
 "*o *****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: DtRRDURQLDqRpLDLqPURUUR (23)
lines: 20
--------------------------------------------------------------------------------
cell from level26cpp_00.txt: 10
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 23
                  g(): 285
                  depth: 25
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "** *x o*\n" // 0
 "* * x  *\n" // 1
 "*hHH H**\n" // 2
 "*  o ***\n" // 3
 "*xo ****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RULUqRURpUURUUpLQtDDpLLD (24)
lines: 19
--------------------------------------------------------------------------------
cell from level26cpp_00.txt: 01
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 22
                  g(): 284
                  depth: 24
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 5

//0123456789
 "*H  x x*\n" // 0
 "*   * H*\n" // 1
 "* H ****\n" // 2
 "*oxho***\n" // 3
 "*o  ****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: RqDPDqUPURURDptLURUURRR (23)
lines: 19
--------------------------------------------------------------------------------
cell from level26cpp_00.txt: 11
--------------------------------------------------------------------------------
                  Solution(s)
                  First State:
                  Push count: 25
                  g(): 229
                  depth: 27
                  Teleport card count: 1
                  Push card count: 3
                  Pull card count: 3

//0123456789
 "**o * **\n" // 0
 "* H  xx*\n" // 1
 "*  H  **\n" // 2
 "*Ho  ***\n" // 3
 "*ohx****\n" // 4
Cost: 0, Heuristic: 0
Depth: 1
steps: ULDQRDtRDDUPUDDRURRqRUURRR (26)
lines: 19
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
    initDecks(world, 20, 14, 4);
}

Level29::Level29()
: TilesetMovable()
, TilesetCardHandler(1) {

}

void Level29::updateTexts([[maybe_unused]]double now) noexcept {
    //textXPos_[0] = screenWidth() / 2 + screenPosX()  - width;
    textXPos_[0] = screenPosX() + 4 * tileWidth();
    textYPos_ = std::max(position().y - tileHeight(), (int)screenPosY());
}

void Level29::drawTexts() noexcept {
    Tileset::drawTexts(27, LevelFactory::minimumPushCount[26]);
}
